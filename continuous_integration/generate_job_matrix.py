#!/usr/bin/env python
# %%
from copy import deepcopy
from itertools import chain, product
import os
import re
from typing import List
import json
import shutil
import requests

from platformio.project.config import ProjectConfig

# %%
# configuration
# boards to *always* skip on PlatformIO
pio_skip_boards = ["esp32-c6-devkitm-1", "arduino_nano_esp32"]
acli_skip_boards = ["uno_pic32", "genuino101"]

# %%
# set verbose
use_verbose = False
if "RUNNER_DEBUG" in os.environ.keys() and os.environ["RUNNER_DEBUG"] == "1":
    use_verbose = True


# %%
# The workspace directory
if "GITHUB_WORKSPACE" in os.environ.keys():
    workspace_dir = os.environ.get("GITHUB_WORKSPACE", os.getcwd())
else:
    workspace_dir = os.getcwd()

if os.path.basename(os.path.normpath(workspace_dir)) == "continuous_integration":
    workspace_dir = os.path.dirname(workspace_dir)
workspace_path = os.path.abspath(os.path.realpath(workspace_dir))
print(f"Workspace Path: {workspace_path}")


# %%
# The examples directory
examples_dir = "./examples/"
examples_path = os.path.join(workspace_dir, examples_dir)
examples_path = os.path.abspath(os.path.realpath(examples_path))
print(f"Examples Path: {examples_path}")


# %%
# The extras directory
extras_dir = "./extras/"
extras_path = os.path.join(workspace_dir, extras_dir)
extras_path = os.path.abspath(os.path.realpath(extras_path))
print(f"Extras Path: {extras_path}")


# %%
# The continuous integration directory
ci_dir = "./continuous_integration/"
ci_path = os.path.join(workspace_dir, ci_dir)
ci_path = os.path.abspath(os.path.realpath(ci_path))
print(f"Continuous Integration Path: {ci_path}")
if not os.path.exists(ci_path):
    print(f"Creating the directory for CI: {ci_path}")
    os.makedirs(ci_path, exist_ok=True)


# %%
# A directory of files to save and upload as artifacts to use in future jobs
artifact_dir = os.path.join(
    os.path.join(workspace_dir, "continuous_integration_artifacts")
)
artifact_path = os.path.abspath(os.path.realpath(artifact_dir))
print(f"Artifact Path: {artifact_path}")
if not os.path.exists(artifact_dir):
    print(f"Creating the directory for artifacts: {artifact_path}")
    os.makedirs(artifact_dir)

# %%
# Set the compiler list
compiler_list = ["arduino-cli", "pio"]


# %%
# Get the examples to build
if "EXAMPLES_TO_BUILD" in os.environ.keys() and os.environ.get(
    "EXAMPLES_TO_BUILD", ""
) not in [
    "all",
    "",
]:
    examples_to_build = [
        example.strip()
        for example in os.environ.get("EXAMPLES_TO_BUILD", "").split(",")
    ]
    if use_verbose:
        print("::debug::Building only examples specified in yaml.")
else:
    # Find all of the examples in the examples folder, append the path "examples" to it
    if use_verbose:
        print("::debug::Building all examples found in the example path.")
    examples_to_build = []
    for root, subdirs, files in chain(os.walk(examples_path), os.walk(extras_path)):
        # print(f"\nSearching for examples in {root}({os.path.split(root)[
        #         -1
        #     ]})\n\t{subdirs}\n\t\t{files}")
        for filename in files:
            file_path = os.path.join(root, filename)
            if filename == os.path.split(root)[-1] + ".ino" and not any(
                e in os.path.normpath(root).split(os.sep)
                for e in [
                    ".history",
                    "archive",
                    "tests",
                    "more",
                ]
            ):
                examples_to_build.append(os.path.relpath(root, workspace_path))
                # print(f"\t- example: {filename} (full path: {file_path})")
                if use_verbose:
                    print(f"::debug::\t- example: {filename} (full path: {file_path})")

# remove any ignored examples from the list
if "EXAMPLES_TO_IGNORE" in os.environ.keys() and os.environ.get(
    "EXAMPLES_TO_IGNORE"
) not in [
    "",
]:
    ex_ignore = os.environ.get("EXAMPLES_TO_IGNORE", "").split(",")
    examples_to_build = [
        example
        for example in examples_to_build
        if not any(
            e in [p.lower() for p in os.path.normpath(example).split(os.sep)]
            for e in [example_.lower().strip() for example_ in ex_ignore]
        )
    ]

if use_verbose:
    print("::debug::==========================================================")
    print("::debug::Building the following Examples:")
    for example in examples_to_build:
        print(f"::debug::Example Name: {example}")
    print("::debug::==========================================================")


# %%
# Pull files to convert between boards and platforms and FQBNs
# Translation between board names on PlatformIO and the Arduino CLI
response = requests.get(
    "https://raw.githubusercontent.com/EnviroDIY/workflows/main/scripts/platformio_to_arduino_boards.json"
)
with open(os.path.join(ci_path, "platformio_to_arduino_boards.json"), "wb") as f:
    f.write(response.content)
with open(os.path.join(ci_path, "platformio_to_arduino_boards.json")) as f:
    pio_to_acli = json.load(f)

# %%
# Arduino CLI configuration
# Always use the generic one from the shared workflow repository
downloaded_arduino_cli_config = False
if "GITHUB_WORKSPACE" in os.environ.keys():
    arduino_cli_config = os.path.join(ci_path, "arduino_cli.yaml")
    arduino_cli_format = "json"
    if not os.path.isfile(arduino_cli_config):
        downloaded_arduino_cli_config = True
        # download the default file
        response = requests.get(
            "https://raw.githubusercontent.com/EnviroDIY/workflows/main/scripts/arduino_cli.yaml"
        )
        # copy to the CI directory
        with open(os.path.join(ci_path, "arduino_cli.yaml"), "wb") as f:
            f.write(response.content)
        # also copy to the artifacts directory
        shutil.copyfile(
            os.path.join(ci_path, "arduino_cli.yaml"),
            os.path.join(artifact_path, "arduino_cli.yaml"),
        )
else:
    arduino_cli_config = os.path.abspath(
        os.path.join(ci_path, "arduino_cli_local.yaml")
    )
    arduino_cli_format = "json"

# %%
# PlatformIO configuration
# If one exists in a "continuous_integration" subfolder of the repository, use it.
# Otherwise, use the generic one from the shared workflow repository
default_pio_config_file = False
pio_config_file = os.path.join(ci_path, "platformio.ini")
if not os.path.isfile(pio_config_file):
    # download the default file
    response = requests.get(
        "https://raw.githubusercontent.com/EnviroDIY/workflows/main/scripts/platformio.ini"
    )
    # make a directory for it and copy it there
    with open(os.path.join(ci_path, "platformio.ini"), "wb") as f:
        f.write(response.content)
    # also copy to the artifacts directory
    shutil.copyfile(
        os.path.join(ci_path, "platformio.ini"),
        os.path.join(artifact_path, "platformio.ini"),
    )
    # mark we're using default
    default_pio_config_file = True

pio_config = ProjectConfig(pio_config_file)
board_to_pio_env = {}
pio_env_to_board = {}
for pio_env_name in pio_config.envs():
    board_to_pio_env[pio_config.get("env:{}".format(pio_env_name), "board")] = (
        pio_env_name
    )
    pio_env_to_board[pio_env_name] = pio_config.get(
        "env:{}".format(pio_env_name), "board"
    )


# %%
# Parse the boards to build
if "BOARDS_TO_BUILD" in os.environ.keys() and os.environ.get(
    "BOARDS_TO_BUILD", ""
) not in [
    "all",
    "",
]:
    boards = [
        board.strip() for board in os.environ.get("BOARDS_TO_BUILD", "").split(",")
    ]
    if use_verbose:
        print("::debug::Building only boards specified in yaml.")
        print(f"::debug::{os.environ.get('BOARDS_TO_BUILD')}")
else:
    boards = list(board_to_pio_env.keys())
    if use_verbose:
        print("::debug::Building all boards available in the platformio.ini file.")
        print(f"::debug::{board_to_pio_env.keys()}")

# remove any ignored boards from the list
if "BOARDS_TO_IGNORE" in os.environ.keys() and os.environ.get(
    "BOARDS_TO_IGNORE", ""
) not in [
    "",
]:
    if use_verbose:
        print("::debug::Ignoring boards specified in yaml.")
    boards = [
        board
        for board in boards
        if board
        not in [
            board_.strip()
            for board_ in os.environ.get("BOARDS_TO_IGNORE", "").split(",")
        ]
    ]

# Make sure we have an equivalent Arduino FQBN or PlatformIO environment for all requested boards
for board in boards:
    if board not in pio_to_acli.keys() and board not in acli_skip_boards:
        print(
            f"""::error:: file=platformio_to_arduino_boards.json,title=No matching Arduino board::
Cannot find matching Arduino FQBN for {board}.
This board will not be compiled with the Arduino CLI
Please check the spelling of your board name or add an entry to the Arduino/PlatformIO board conversion file."""
        )
        boards.remove(board)
    if board not in board_to_pio_env.keys() and board not in pio_skip_boards:
        print(
            f"""::warning file=platformio.ini,title=No PlatformIO Environment::
No matching environment was found in the platformio.ini file for {board}.
This board will be compiled with no reference to a specific environment.
Please check the spelling of your board name or add an entry to your platformio.ini if this is not your expected behavior."""
        )


# %%
# Generate a list of currently supported modems from the client file
client_file = open(os.path.join(workspace_path, "src/TinyGsmClient.h"), "r")
client_file_contents = client_file.read()
client_file.close()

modem_list = []
# if defined(TINY_GSM_MODEM_SIM800)
# NOTE: in cases where there are multiple #if defined() statements on the same line, this will only find the first one.
# This is what we want, because we only want to build one example per modem, not one example per #if defined() statement.
pattern = re.compile(
    r"^(?:#if|#elif) defined[\s\(](?P<define>TINY_GSM_MODEM_\w+)",
    re.MULTILINE,
)
# find matches and add them to the lists
for match in re.finditer(pattern, client_file_contents):
    # print(match.group("define"))
    modem_list.append(match.group("define"))


# %%
# expand the combination of boards, modems, and examples into a job matrix
cart_join = list(product(*[compiler_list, examples_to_build, boards, modem_list]))
print(f"Total possible combinations: {len(cart_join)}")


# %%
# Exclude modems without SSL support from HttpsClient and AWS_IoTCore examples
unsecured_modems = [
    "TINY_GSM_MODEM_SIM900",
    "TINY_GSM_MODEM_SIM7000",
    "TINY_GSM_MODEM_SIM5320",
    "TINY_GSM_MODEM_SIM5360",
    "TINY_GSM_MODEM_SIM5300",
    "TINY_GSM_MODEM_SIM7100",
    "TINY_GSM_MODEM_M95",
    "TINY_GSM_MODEM_BG95",
    "TINY_GSM_MODEM_A6",
    "TINY_GSM_MODEM_A7",
    "TINY_GSM_MODEM_M590",
    "TINY_GSM_MODEM_MC60",
    "TINY_GSM_MODEM_MC60E",
]

# a list of known failures to skip in the job matrix
matrix_exclusions = [
    {
        "compilers": compiler_list,
        "examples": [os.path.join("examples", "BlynkClient")],
        "boards": [
            "nona4809",
            "nano_nora",
            "nano_every",
            "due",
            "dueUSB",
        ],  # these processors are not supported by the Blynk library
        "modems": deepcopy(modem_list),
    },
    {
        "compilers": compiler_list,
        "examples": [os.path.join("examples", "BlynkClient")],
        "boards": boards,
        "modems": [
            modem
            for modem in modem_list
            if modem
            in [
                "TINY_GSM_MODEM_ESP32",
                "TINY_GSM_MODEM_ESP8266",
                "TINY_GSM_MODEM_ESP8266_NONOS",
            ]  # TinyGSM wifi modems are not supported by the Blynk library
        ],
    },
    {
        "compilers": compiler_list,
        "examples": [os.path.join("examples", "BlynkClient")],
        "boards": ["leonardo", "feather32u4", "yun"],
        "modems": ["TINY_GSM_MODEM_XBEE"],
        # The XBee is a few bytes too big for 32u4 boards
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("examples", "AllFunctions"),
            os.path.join("extras", "tools", "test_build"),
        ],
        "boards": [
            "uno",
            "leonardo",
            "yun",
            "feather328p",
            "feather32u4",
        ],  # doesn't fit on 328p or 32u4
        "modems": deepcopy(modem_list),
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("extras", "tools", "test_build"),
        ],
        "boards": [
            "uno_pic32",
            "leonardo",
            "yun",
            "feather328p",
            "feather32u4",
        ],  # doesn't fit
        "modems": ["TINY_GSM_MODEM_SARAR5"],
    },
    {
        "compilers": compiler_list,
        "examples": [os.path.join("examples", "AWS_IoTCore")],
        "boards": [
            "uno",
            "leonardo",
            "yun",
            "feather328p",
            "feather32u4",
        ],  # doesn't fit on 328p or 32u
        "modems": [modem for modem in modem_list if modem not in unsecured_modems],
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("examples", "HttpsClient"),
            os.path.join("examples", "AWS_IoTCore"),
        ],
        "boards": boards,
        "modems": [modem for modem in modem_list if modem in unsecured_modems],
        # Exclude SSL-incapable modems on SSL required examples.
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("extras", "tools", "AT_Debug"),
            os.path.join("extras", "tools", "AT_Spy"),
        ],  # never build these examples in CI, they are for debugging only
        "boards": boards,
        "modems": deepcopy(modem_list),
    },
]

# expand the matrix exclusions to a list of tuples for easier filtering
expanded_matrix_exclusions = []
for exclusion in matrix_exclusions:
    exclusion_list = list(
        product(
            *[
                exclusion["compilers"],
                exclusion["examples"],
                exclusion["boards"],
                exclusion["modems"],
            ]
        )
    )
    for i in range(len(exclusion_list)):
        expanded_matrix_exclusions.append(
            (
                exclusion_list[i][0],
                exclusion_list[i][1],
                exclusion_list[i][2],
                exclusion_list[i][3],
            )
        )

# filter out the known failures from the job matrix
expanded_matrix_exclusions_set = set(expanded_matrix_exclusions)
print(f"Matrix exclusions: {len(expanded_matrix_exclusions_set)}")

# %%
# eh, minimize the matrix instead of maximizing
matrix_inclusions = [
    {
        "compilers": compiler_list,
        "examples": [os.path.join("extras", "tools", "test_build")],
        "boards": [
            board
            for board in boards
            if board
            not in [
                "uno",
                "leonardo",
                "yun",
                "feather328p",
                "feather32u4",
            ]  # doesn't fit on 328p or 32u
        ],
        "modems": deepcopy(modem_list),
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("examples", "WebClient"),
            os.path.join("examples", "MqttClient"),
        ],
        "boards": [  # boards too small for the bigger test build
            "uno",
            "leonardo",
            "yun",
            "feather328p",
            "feather32u4",
        ],
        "modems": deepcopy(modem_list),
    },
    {
        "compilers": compiler_list,
        "examples": [
            os.path.join("examples", "WebClient"),
            os.path.join("examples", "MqttClient"),
        ],
        "boards": [  # boards too small for the bigger test build
            "uno_pic32",
        ],
        "modems": ["TINY_GSM_MODEM_SARAR5"],
    },
    {
        "compilers": compiler_list,
        "examples": examples_to_build,
        "boards": [
            "mayfly",
            "stonefly",
            "mega",
            "zeroUSB",
            "uno_r4_wifi",
            "nodemcu",
            "esp32dev",
            "esp32-c3-devkitm-1",
            "esp32-c6-devkitm-1",
            "esp32-s3-devkitm-1",
        ],
        "modems": deepcopy(modem_list),
    },
]

# expand the matrix inclusions to a list of tuples for easier filtering
expanded_matrix_inclusions = []
for inclusion in matrix_inclusions:
    inclusion_list = list(
        product(
            *[
                inclusion["compilers"],
                inclusion["examples"],
                inclusion["boards"],
                inclusion["modems"],
            ]
        )
    )
    for i in range(len(inclusion_list)):
        expanded_matrix_inclusions.append(
            (
                inclusion_list[i][0],
                inclusion_list[i][1],
                inclusion_list[i][2],
                inclusion_list[i][3],
            )
        )

# remove duplicates from the inclusions list
expanded_matrix_inclusions_set = set(expanded_matrix_inclusions)
print(f"Expanded matrix inclusions: {len(expanded_matrix_inclusions_set)}")

# %%
# decide on the filtered matrix to use for the job matrix
filtered_matrix = [
    e
    for e in cart_join
    if e not in expanded_matrix_exclusions_set and e in expanded_matrix_inclusions_set
]
filtered_matrix = sorted(
    filtered_matrix, key=lambda x: (x[2], x[3], x[1], x[0])
)  # sort by board, modem, example, compiler
print(f"Final filtered matrix: {len(filtered_matrix)}")


# %%
# helper functions to create commands
def create_arduino_cli_compile_command(
    code_subfolder: str,
    fqbn: str,
    extra_flags: List[str] = [],
) -> str:
    arduino_command_args = [
        "arduino-cli",
        "compile",
    ]
    if use_verbose:
        arduino_command_args += ["--verbose"]
    arduino_command_args += [
        "--warnings",
        "more",
        "--config-file",
        f'"{arduino_cli_config}"',
        "--format",
        f"{arduino_cli_format}",
        "--fqbn",
        fqbn,
    ]
    if len(extra_flags) > 0:
        arduino_command_args += [
            "--build-property",
            "compiler.cpp.extra_flags=" + " ".join(extra_flags),
        ]
    arduino_command_args += [
        f'"{os.path.join(workspace_path, code_subfolder)}"',
    ]
    return " ".join(arduino_command_args)


def create_pio_ci_compile_command(
    code_subfolder: str,
    pio_board_or_env: str | List[str],
    use_pio_config_file: bool,
    extra_flags: List[str] = [],
    use_run: bool = False,
) -> str:
    pio_command_args = [
        "pio",
        "run" if use_run else "ci",
    ]
    if use_verbose:
        pio_command_args += ["--verbose"]
    if use_pio_config_file:
        pio_command_args += ["--project-conf", f'"{pio_config_file}"']
        if type(pio_board_or_env) == str:
            pio_command_args += ["--environment", pio_board_or_env]
        else:
            for pio_board_or_env_item in pio_board_or_env:
                pio_command_args += [
                    "--environment",
                    pio_board_or_env_item,
                ]
    elif not use_run:
        if type(pio_board_or_env) == str:
            pio_command_args += [
                "--board",
                pio_board_or_env,
            ]
        else:
            for pio_board_or_env_item in pio_board_or_env:
                pio_command_args += [
                    "--board",
                    pio_board_or_env_item,
                ]
    else:
        raise ValueError(
            "you must be using a pio config file if you are using the 'run' command"
        )
    if use_run:
        pio_command_args += [
            "--project-dir",
            f'"{os.path.realpath(os.path.join(artifact_dir, "pio_ci_build"))}"',
        ]
    if (
        len(extra_flags) > 0 and not use_pio_config_file and not use_run
    ):  # these CANNOT be used with a pio config file
        pio_command_args += [
            "--project-option",
            f'"build_flags = {' '.join(extra_flags)}"',
        ]
    else:
        if len(extra_flags) > 0 and use_pio_config_file and not use_run:
            print(
                "Warning: extra_flags are being ignored because you are using a pio config file."
            )
        elif len(extra_flags) > 0 and use_run:
            print(
                "Warning: extra_flags are being ignored because you are using a pio run command."
            )
    if not use_run:
        pio_command_args += [
            f'"{os.path.join(workspace_path, code_subfolder)}"',
        ]

    return " ".join(pio_command_args)


def get_filename_slug(job_key, value) -> str:
    if job_key in ["compiler", "board", "flag"]:
        return value.replace("_", "-")
    if job_key == "modem":
        return value.replace("TINY_GSM_MODEM_", "").replace("_", "-")
    if job_key == "example":
        return value.rsplit(os.path.sep)[-1].replace("_", "-")
    else:
        raise ValueError(
            f"Invalid job key: {job_key}. Must be one of ['compiler', 'modem', 'board', 'example']"
        )


def get_filename_for_log(job: dict) -> str:
    if "compiler" in job:
        compiler = job["compiler"]
    else:
        compiler = "arduino-cli" if "arduino-cli" in job["command"][0] else "pio"
    c_name = get_filename_slug("compiler", compiler)
    m_name = get_filename_slug("modem", job["modem"])
    b_name = get_filename_slug("board", job["board"])
    ex_name = get_filename_slug("example", job["example"])
    extension = "json" if compiler == "arduino-cli" else "log"
    return os.path.abspath(
        os.path.join(
            artifact_path,
            f"{c_name}_{m_name}_{b_name}_{ex_name}.{extension}",
        )
    )


def get_job_info_from_filename(filename: str) -> dict:
    name_parts = os.path.basename(filename).split("_")
    if len(name_parts) == 3:
        return {
            "compiler": name_parts[0],
            "board": name_parts[1],
            "example": name_parts[2].rsplit(".", 1)[0],
        }
    else:
        return {
            "compiler": name_parts[0],
            "modem": name_parts[1],
            "board": name_parts[2],
            "example": name_parts[3].rsplit(".", 1)[0],
        }


def group_and_log_commands(
    commands: List[str], group_title: str, output_filename: str
) -> List[str]:
    command_list = []
    command_list.append("\necho ::group::{}".format(group_title))
    command_list.append("group_failed=0")
    for command in commands:
        command_list.append(command + ' 2>&1 | tee -a "{}"'.format(output_filename))
        command_list.append("result_code=${PIPESTATUS[0]}")
        command_list.append(
            'if [ "$result_code" -ne "0" ]; then group_failed=1; status=1; fi'
        )
    # command_list.append(
    #     f'if [ "$group_failed" -eq "0" ]; then echo -e " - {group_title} :white_check_mark:" >> $GITHUB_STEP_SUMMARY; else echo -e " - {group_title} :x:" >> $GITHUB_STEP_SUMMARY; fi'
    # )
    command_list.append("echo ::endgroup::")
    command_list.append(
        f'if [ "$group_failed" -eq "0" ]; then echo -e "\\e[32m{group_title} successfully compiled\\e[0m"; else echo -e "\\e[31m{group_title} failed to compile\\e[0m"; fi'
    )
    return command_list


def create_command_list_from_matrix(matrix_item: tuple, **kwargs):
    if len(matrix_item) != 4:
        raise ValueError(
            "Matrix item must be a tuple of length 4 (compiler, example, board, modem)"
        )
    compiler, example, board, modem = matrix_item
    job_dict = {
        "compiler": compiler,
        "modem": modem,
        "board": board,
        "example": example,
    }
    output_file_name = get_filename_for_log(job_dict)
    if compiler == "arduino-cli":
        if board not in pio_to_acli.keys() or board in acli_skip_boards:
            # return [
            #     f"echo 'Skipping {example} for {board} because no matching Arduino FQBN was found.'"
            # ]
            return None
        fqbn = pio_to_acli[board]["fqbn"]
        build_command = create_arduino_cli_compile_command(
            code_subfolder=example, fqbn=fqbn, **kwargs
        )
    elif compiler == "pio":
        if board in pio_skip_boards:
            # return [
            #     f"echo 'Skipping {example} for {board} because it is in the list of boards to skip for PlatformIO.'"
            # ]
            return None
        if board in board_to_pio_env.keys():
            pio_board_or_env = board_to_pio_env[board]
            use_pio_config_file = True
        else:
            pio_board_or_env = board
            use_pio_config_file = False
        build_command = create_pio_ci_compile_command(
            code_subfolder=example,
            pio_board_or_env=pio_board_or_env,
            use_pio_config_file=use_pio_config_file,
            **kwargs,
        )
    else:
        raise ValueError("Invalid compiler provided.")

    example_name = f"{os.path.split(example)[-1]}"
    example_full_path = os.path.join(workspace_path, example, example_name + ".ino")
    sed_comment = f"sed -i 's/#define TINY_GSM_MODEM_/\\/\\/ #define TINY_GSM_MODEM_/g' \"{example_full_path}\""
    sed_addition = f"sed -i '1i\\\n#define {modem}\\\n' \"{example_full_path}\""

    job_dict["output_file_name"] = output_file_name
    job_dict["build_commands"] = [sed_comment, sed_addition, build_command]

    return deepcopy(job_dict)


# %%
# convert the matrix into a list of command for each board and modem combination
complete_command_matrix: List[dict] = []
for matrix_item in filtered_matrix:
    command_block = create_command_list_from_matrix(matrix_item=matrix_item)
    if command_block is not None:
        complete_command_matrix.append(command_block)


# %%
# group the commands by how we want the collapsing in the logs to work
log_groupers = ["compiler", "board", "example", "modem"]
grouped_command_matrix: dict[str, dict[str, str | List[str]]] = {}
for matrix_item in complete_command_matrix:
    l_names = []
    for grouper in log_groupers:
        if grouper not in matrix_item.keys():
            raise ValueError(
                f"Matrix item {matrix_item} does not have the key {grouper}"
            )
        elif matrix_item[grouper] is None:
            raise ValueError(
                f"Matrix item {matrix_item} has a None value for the key {grouper}"
            )
        else:
            l_names.append(get_filename_slug(grouper, matrix_item[grouper]))
    l_key = "-".join(l_names)
    l_command_list = group_and_log_commands(
        matrix_item["build_commands"],
        group_title=l_key,
        output_filename=matrix_item["output_file_name"],
    )
    if l_key not in grouped_command_matrix.keys():
        l_dict: dict[str, str | List[str]] = {
            "log_group": l_key,
            "group_commands": l_command_list,
        }
        for grouper in log_groupers:
            l_dict[grouper] = matrix_item[grouper]
        grouped_command_matrix[l_key] = l_dict
    else:
        grouped_command_matrix[l_key]["group_commands"] += l_command_list  # type: ignore
print(f"Total log groups: {len(grouped_command_matrix)}")


# %%
# group the commands into jobs
start_job_commands: List[str] = ["status=0"]
end_job_commands: List[str] = ["\n\nexit $status"]
job_groupers = ["compiler", "board"]
grouped_job_matrix = {}
for l_key, group_dict in grouped_command_matrix.items():
    j_names = []
    for grouper in job_groupers:
        if grouper not in group_dict.keys():
            raise ValueError(
                f"Matrix item {group_dict} does not have the key {grouper}"
            )
        elif group_dict[grouper] is None:
            raise ValueError(
                f"Matrix item {group_dict} has a None value for the key {grouper}"
            )
        else:
            j_names.append(get_filename_slug(grouper, group_dict[grouper]))
    job_name = " - ".join(j_names)
    job_tag = "-".join(j_names)
    if job_tag not in grouped_job_matrix.keys():
        j_dict: dict[str, str | List[str]] = {
            "job_name": job_name,
            "job_tag": job_tag.lower(),
            "job_command": group_dict["group_commands"],
        }
        for grouper in log_groupers:
            j_dict[grouper] = group_dict[grouper]
        grouped_job_matrix[job_tag] = j_dict
    else:
        grouped_job_matrix[job_tag]["job_command"] += group_dict["group_commands"]
print(f"Total jobs: {len(grouped_job_matrix)}")


# %%
# Convert commands in the matrix into bash scripts
for job_tag, matrix_job in grouped_job_matrix.items():
    bash_file_name = job_tag + ".sh"
    print(f"Writing bash file to {os.path.join(artifact_path, bash_file_name)}")
    bash_out = open(os.path.join(artifact_path, bash_file_name), "w+")
    bash_out.write("#!/bin/bash\n\n")
    bash_out.write("""
set -e # Exit with nonzero exit code if anything fails
if [ "$RUNNER_DEBUG" = "1" ]; then
    echo "Enabling debugging!"
    set -v # Prints shell input lines as they are read.
    set -x # Print command traces before executing command.
fi

""")
    bash_out.write("\n".join(matrix_job["job_command"]))
    bash_out.close()
    matrix_job["script"] = os.path.join(artifact_path, bash_file_name)

# Remove the command from the dictionaries before outputting them
arduino_job_matrix = [
    {
        vk: vv
        for vk, vv in v.items()
        if vk == "job_name" or vk == "job_tag" or vk == "script"
    }
    for k, v in grouped_job_matrix.items()
    if v["compiler"] == "arduino-cli"
]
pio_job_matrix = [
    {
        vk: vv
        for vk, vv in v.items()
        if vk == "job_name" or vk == "job_tag" or vk == "script"
    }
    for k, v in grouped_job_matrix.items()
    if v["compiler"] == "pio"
]

# %%
# Write out output
print(
    'echo "arduino_job_matrix={}" >> $GITHUB_OUTPUT'.format(
        json.dumps(arduino_job_matrix)
    )
)
json_out = open(os.path.join(artifact_dir, "arduino_job_matrix.json"), "w+")
json.dump(arduino_job_matrix, json_out, indent=2)
json_out.close()

print('echo "pio_job_matrix={}" >> $GITHUB_OUTPUT'.format(json.dumps(pio_job_matrix)))
json_out = open(os.path.join(artifact_dir, "pio_job_matrix.json"), "w+")
json.dump(pio_job_matrix, json_out, indent=2)
json_out.close()


# %%
# different attempt to save output
if "GITHUB_WORKSPACE" in os.environ.keys():
    with open(os.environ["GITHUB_OUTPUT"], "a") as fh:
        print("arduino_job_matrix={}".format(json.dumps(arduino_job_matrix)), file=fh)
        print("pio_job_matrix={}".format(json.dumps(pio_job_matrix)), file=fh)


# %%
if "GITHUB_WORKSPACE" not in os.environ.keys():
    try:
        print("Deleting artifact directory")
        shutil.rmtree(artifact_dir)
    except:
        pass
    try:
        print("Deleting downloaded jsons")
        os.remove(
            os.path.join(ci_path, "platformio_to_arduino_boards.json")
        )  # remove downloaded file
        os.rmdir(ci_path)  # remove dir if empty
    except:
        pass
    if downloaded_arduino_cli_config:
        try:
            print("Deleting default Arduino CLI file")
            os.remove(arduino_cli_config)  # remove downloaded file
            os.rmdir(ci_path)  # remove dir if empty
        except:
            pass
    if default_pio_config_file:
        try:
            print("Deleting default_pio_config_file")
            os.remove(pio_config_file)  # remove downloaded file
            os.rmdir(ci_path)  # remove dir if empty
        except:
            pass


# %%
# cSpell:words devkitm acli genuino bluepill fqbn fqbns pipestatus jsons endgroup DTINY_GSM_RX_BUFFER Wextra
