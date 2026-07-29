from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SRC_DIR = ROOT / "src"

TEMPLATE_CLASS_REFS = {
    "TinyGsmModem": "TinyGsmModem<modemType, modemConfig>",
    "TinyGsmGPRS": "TinyGsmGPRS<modemType>",
    "TinyGsmTCP": "TinyGsmTCP<modemType, tcpConfig>",
    "TinyGsmCalling": "TinyGsmCalling<modemType>",
    "TinyGsmSMS": "TinyGsmSMS<modemType>",
    "TinyGsmTime": "TinyGsmTime<modemType>",
    "TinyGsmBattery": "TinyGsmBattery<modemType>",
    "TinyGsmGPS": "TinyGsmGPS<modemType>",
    "TinyGsmGSMLocation": "TinyGsmGSMLocation<modemType>",
    "TinyGsmNTP": "TinyGsmNTP<modemType>",
    "TinyGsmTemperature": "TinyGsmTemperature<modemType>",
    "TinyGsmBluetooth": "TinyGsmBluetooth<modemType>",
    "TinyGsmSSL": "TinyGsmSSL<modemType>",
}


class ClassInfo:
    def __init__(
        self, name: str, path: Path, base_names: list[str], class_ref: str
    ) -> None:
        self.name = name
        self.path = path
        self.base_names = base_names
        self.class_ref = class_ref


class FunctionEntry:
    def __init__(
        self,
        name: str,
        supported: bool,
        group: str,
        source_name: str,
        owner_class: str,
    ) -> None:
        self.name = name
        self.supported = supported
        self.group = group
        self.source_name = source_name
        self.owner_class = owner_class


def read_text(path: Path) -> str:
    return path.read_text(encoding="utf-8")


def write_text(path: Path, text: str) -> None:
    path.write_text(text, encoding="utf-8")


def find_class_decl(text: str) -> tuple[str, list[str]] | None:
    match = re.search(
        r"^\s*class\s+(TinyGsm\w+)\b.*?\{", text, re.MULTILINE | re.DOTALL
    )
    if not match:
        return None

    class_name = match.group(1)
    decl_text = text[match.start() : match.end()]
    base_names = []
    for base in re.findall(r"TinyGsm\w+", decl_text):
        if base != class_name and base not in base_names:
            base_names.append(base)
    return class_name, base_names


def class_ref_for(class_name: str) -> str:
    return TEMPLATE_CLASS_REFS.get(class_name, class_name)


def extract_class_body(text: str, class_name: str) -> str | None:
    class_match = re.search(
        rf"^\s*class\s+{re.escape(class_name)}\b.*?\{{", text, re.MULTILINE | re.DOTALL
    )
    if not class_match:
        return None

    start = class_match.end() - 1
    depth = 0
    in_string = False
    in_char = False
    escape = False
    i = start
    while i < len(text):
        ch = text[i]
        nxt = text[i + 1] if i + 1 < len(text) else ""

        if escape:
            escape = False
        elif ch == "\\" and (in_string or in_char):
            escape = True
        elif ch == '"' and not in_char:
            in_string = not in_string
        elif ch == "'" and not in_string:
            in_char = not in_char
        elif not in_string and not in_char:
            if ch == "/" and nxt == "/":
                end = text.find("\n", i)
                if end == -1:
                    break
                i = end
                continue
            if ch == "/" and nxt == "*":
                end = text.find("*/", i + 2)
                if end == -1:
                    break
                i = end + 2
                continue
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
                if depth == 0:
                    return text[start + 1 : i]

        i += 1

    return None


def normalize_signature(buffer: list[str]) -> str:
    text = " ".join(buffer)
    text = re.sub(r"//.*", "", text)
    text = re.sub(r"/\*.*?\*/", " ", text)
    text = re.sub(r"\s+", " ", text).strip()
    return text


def extract_member_name(signature: str, class_name: str | None = None) -> str | None:
    if not signature:
        return None
    if any(
        token in signature
        for token in (
            "friend class",
            "using ",
            "typedef ",
            "template <",
            "class ",
            "struct ",
            "enum ",
        )
    ):
        return None
    if "(" not in signature:
        return None

    prefix = signature.split("(", 1)[0].strip()
    if not prefix:
        return None

    if class_name and re.search(rf"\b{re.escape(class_name)}\s*$", prefix):
        return None

    matches = list(re.finditer(r"(~?operator\s*[^\s(]+|~?[A-Za-z_]\w*)", prefix))
    if not matches:
        return None

    return re.sub(r"\s+", " ", matches[-1].group(1).strip())


def parse_group_heading(comment_text: str) -> str | None:
    if not comment_text:
        return None
    if any(
        token in comment_text
        for token in ("@brief", "@param", "@return", "@tparam", "@copydetails", "@fn")
    ):
        return None

    name_match = re.search(r"@name\s+(.+)", comment_text)
    if name_match:
        return name_match.group(1).strip()

    lines = []
    for raw_line in comment_text.splitlines():
        line = raw_line.strip().lstrip("/*").lstrip("*").rstrip("*/").strip()
        if not line or line.startswith("@"):
            continue
        lines.append(line)

    if lines:
        return lines[0]
    return None


def is_member_start(line: str) -> bool:
    if not line:
        return False
    if line.startswith(("//", "/*", "*", "*/")):
        return False
    if line in {"public:", "protected:", "private:"}:
        return False
    if line.startswith(
        (
            "friend ",
            "using ",
            "typedef ",
            "class ",
            "struct ",
            "enum ",
            "template ",
            "#",
        )
    ):
        return False
    return "(" in line or "TINY_GSM_ATTR_NOT_" in line


def collect_wrappers(
    text: str, class_name: str, class_ref: str, source_name: str
) -> list[FunctionEntry]:
    body = extract_class_body(text, class_name)
    if body is None:
        return []

    entries: list[FunctionEntry] = []
    seen: set[str] = set()

    access = "private"
    depth = 1
    capturing = False
    buffer: list[str] = []
    pending_group: str | None = None
    current_group: str | None = None
    in_comment = False
    comment_buffer: list[str] = []

    def finalize_member(signature: str) -> None:
        nonlocal pending_group, current_group
        if access != "public":
            return

        name = extract_member_name(signature, class_name)
        if not name or name.startswith("operator"):
            return

        is_unsupported = (
            "TINY_GSM_ATTR_NOT_AVAILABLE" in signature
            or "TINY_GSM_ATTR_NOT_IMPLEMENTED" in signature
        )
        group = pending_group or current_group or "Other functions"
        if pending_group:
            current_group = pending_group
            pending_group = None

        if name in seen:
            return
        seen.add(name)
        entries.append(
            FunctionEntry(name, not is_unsupported, group, source_name, class_ref)
        )

    for raw_line in body.splitlines():
        line = raw_line.rstrip()
        stripped = line.strip()

        if depth == 1 and in_comment:
            comment_buffer.append(line)
            if "*/" in stripped:
                in_comment = False
                heading = parse_group_heading("\n".join(comment_buffer))
                if heading:
                    pending_group = heading
                comment_buffer = []
            continue

        if depth == 1 and stripped.startswith("/*"):
            in_comment = True
            comment_buffer = [line]
            if "*/" in stripped:
                in_comment = False
                heading = parse_group_heading("\n".join(comment_buffer))
                if heading:
                    pending_group = heading
                comment_buffer = []
            continue

        if depth == 1 and stripped in {"public:", "protected:", "private:"}:
            access = stripped[:-1]
            continue

        if depth == 1:
            if capturing:
                buffer.append(line)
                if "{" in stripped or ";" in stripped:
                    signature = normalize_signature(buffer)
                    finalize_member(signature)
                    capturing = False
                    buffer = []
            elif is_member_start(stripped):
                capturing = True
                buffer = [line]
                if "{" in stripped or ";" in stripped:
                    signature = normalize_signature(buffer)
                    finalize_member(signature)
                    capturing = False
                    buffer = []

        depth += line.count("{") - line.count("}")

    return entries


def build_class_index(files: list[Path]) -> dict[str, ClassInfo]:
    index: dict[str, ClassInfo] = {}
    for path in files:
        text = read_text(path)
        decl = find_class_decl(text)
        if not decl:
            continue
        class_name, base_names = decl
        index[class_name] = ClassInfo(
            class_name, path, base_names, class_ref_for(class_name)
        )
    return index


def collect_recursive(
    class_name: str, index: dict[str, ClassInfo], visited: set[str] | None = None
) -> list[FunctionEntry]:
    if visited is None:
        visited = set()
    if class_name in visited:
        return []
    visited.add(class_name)

    info = index.get(class_name)
    if not info:
        return []

    entries: dict[str, FunctionEntry] = {}
    used_lower: set[str] = set()

    for base in info.base_names:
        for entry in collect_recursive(base, index, visited):
            lower_name = entry.name.lower()
            entries[entry.name] = entry
            used_lower.add(lower_name)

    text = read_text(info.path)
    for entry in collect_wrappers(text, class_name, info.class_ref, info.path.name):
        lower_name = entry.name.lower()
        if lower_name in used_lower:
            continue
        existing = entries.get(entry.name)
        if existing is None:
            entries[entry.name] = entry
            used_lower.add(lower_name)
        else:
            existing.supported = entry.supported

    return list(entries.values())


def format_list(items: list[FunctionEntry], indent: str = " *     - ") -> list[str]:
    return [
        f'{indent}@ref {item.owner_class}::{item.name} "{item.name}()"'
        for item in items
    ]


def build_doc_block(entries: list[FunctionEntry]) -> str:
    grouped: list[tuple[str, list[FunctionEntry], list[FunctionEntry]]] = []
    group_index: dict[str, int] = {}

    for entry in entries:
        group_label = entry.group
        if entry.source_name.endswith(".tpp"):
            group_label = f"{group_label} ({entry.source_name})"

        if group_label not in group_index:
            group_index[group_label] = len(grouped)
            grouped.append((group_label, [], []))

        idx = group_index[group_label]
        supported, unsupported = grouped[idx][1], grouped[idx][2]
        if entry.supported:
            supported.append(entry)
        else:
            unsupported.append(entry)

    lines = [" * # Supported Public Functions", " *"]
    if not grouped:
        lines.append(" * - None documented.")
    for group_label, supported, unsupported in grouped:
        lines.append(f" * - {group_label}")
        if supported and unsupported:
            lines.append(" *   - Supported functions:")
        if supported:
            lines.extend(format_list(supported))
        if supported and unsupported:
            lines.append(" *   - Unavailable functions:")
        if unsupported:
            lines.extend(format_list(unsupported))

    lines.append(" *")
    return "\n".join(lines)


def update_file(path: Path, index: dict[str, ClassInfo]) -> bool:
    text = read_text(path)
    decl = find_class_decl(text)
    if not decl:
        return False

    class_name, _ = decl
    if class_name not in index:
        return False

    top_match = re.search(r"(?s)/\*\*\s*\r?\n.*?@defgroup\s+.*?\*/", text)
    if not top_match:
        return False

    start = text.find("# Supported Public Functions", top_match.start())
    if start == -1:
        start = text.find("## Supported Features", top_match.start())
    conn = text.find("# Connection Information", top_match.start())
    if start == -1 or conn == -1 or conn <= start:
        return False

    line_start = text.rfind("\n", 0, start)
    if line_start == -1:
        line_start = 0
    else:
        line_start += 1

    conn_line_start = text.rfind("\n", 0, conn)
    if conn_line_start == -1:
        conn_line_start = 0
    else:
        conn_line_start += 1

    entries = collect_recursive(class_name, index)
    new_block = build_doc_block(entries)

    conn_block = text[conn_line_start:]
    if conn_block.startswith("# Connection Information"):
        conn_block = " * " + conn_block

    new_text = text[:line_start] + new_block + "\n" + conn_block
    if new_text == text:
        return False

    write_text(path, new_text)
    return True


def main() -> None:
    files = [
        path
        for path in list(SRC_DIR.glob("TinyGsmClient*.h"))
        + list(SRC_DIR.glob("TinyGsm*.tpp"))
        if path.name
        not in {"TinyGsmClient.h", "TinyGsmClientEspressif.h", "TinyGsmCommon.h"}
    ]
    index = build_class_index(files)

    updated = []
    for path in files:
        if update_file(path, index):
            updated.append(path.name)

    for name in updated:
        print(f"Updated {name}")
    if not updated:
        print("No files updated")


if __name__ == "__main__":
    main()
