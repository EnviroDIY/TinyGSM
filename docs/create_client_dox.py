# %%
import os
import re, mmap
from string import Template

# %%
client_src_files = [
    os.path.join(dp, f)
    for dp, dn, filenames in os.walk(
        "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\src"
    )
    for f in filenames
    if "TinyGsmClient" in f and "TinyGsmClient.h" not in f
]

# %%
# Delete any previously generated files
prev_generated_files = [
    os.path.join(dp, f)
    for dp, dn, filenames in os.walk(
        "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\docs"
    )
    for f in filenames
    if "clients_" in f and f.endswith(".dox")
]
for prev_file in prev_generated_files:
    os.remove(prev_file)

# %%

for client_file in client_src_files:
    # get the base name of the client file
    base_name = os.path.basename(client_file)
    modem_abbrev = base_name.replace("TinyGsmClient", "").replace(".h", "")

    modem_class_name = "TinyGsm" + modem_abbrev
    client_class_name = "GsmClient" + modem_abbrev
    has_secure_client_class = False
    secure_client_class_name = "GsmClientSecure" + modem_abbrev
    has_secure_ctor_macro = False

    # open the source file to check for class names
    re_modem = r"^class\s+(?P<modem_class>TinyGsm\w+)"
    re_client = r"\s{2,}class\s+(?P<client_class>GsmClient\w+)"
    re_secure_client = r"\s{2,}class\s+(?P<secure_client_class>GsmClientSecure\w+)"
    with open(client_file, "r", encoding="UTF-8") as source_file:
        source_content = source_file.read()
        modem_search = re.search(re_modem, source_content, re.MULTILINE)
        client_search = re.search(re_client, source_content)
        secure_client_search = re.search(re_secure_client, source_content)
        if modem_search is not None:
            modem_class_name = modem_search.group("modem_class")
            print(f"Found modem class {modem_class_name} in {client_file}")
        else:
            print(f"**Warning**: No modem class found in {client_file}")
            continue
        if client_search is not None:
            client_class_name = client_search.group("client_class")
            print(f"Found client class {client_class_name} in {client_file}")
        else:
            print(f"**Warning**: No client class found for {modem_class_name}")
            continue
        if secure_client_search is not None:
            secure_client_class_name = secure_client_search.group("secure_client_class")
            has_secure_client_class = True
            print(
                f"Found secure client class {secure_client_class_name} in {client_file}"
            )
        if "TINY_GSM_SECURE_CLIENT_CTORS" in source_content:
            has_secure_ctor_macro = True

    # create a dictionary for substitutions
    sub_dict = {
        "modem_class": modem_class_name,
        "client_class": client_class_name,
        "secure_client_class": secure_client_class_name,
    }

    # src_template = "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\docs\\DOC_TEMPLATE_clients.dox_TEMPLATE"
    # destination = (
    #     src_template.replace("DOC_TEMPLATE_", "")
    #     .replace("clients_", "clients_" + modem_abbrev + "_")
    #     .replace(".dox_TEMPLATE", "_" + modem_abbrev + ".dox")
    # )

    # # open the template file
    # with open(src_template, "r", encoding="UTF-8") as template_file:
    #     original = template_file.read()

    #     # make substitutions
    #     new_data = Template(original).substitute(sub_dict)

    #     # write the new file
    #     with open(destination, "w", encoding="UTF-8") as dest_file:
    #         dest_file.write(new_data)

    # if has_secure_client_class:
    #     src_template = "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\docs\\DOC_TEMPLATE_clients_secure_fxns.dox_TEMPLATE"
    #     destination = (
    #         src_template.replace("DOC_TEMPLATE_", "")
    #         .replace("clients_", "clients_" + modem_abbrev + "_")
    #         .replace(".dox_TEMPLATE", "_" + modem_abbrev + ".dox")
    #     )

    #     # open the template file
    #     with open(src_template, "r", encoding="UTF-8") as template_file:
    #         original = template_file.read()

    #         # make substitutions
    #         new_data = Template(original).substitute(sub_dict)

    #         # write the new file
    #         with open(destination, "w", encoding="UTF-8") as dest_file:
    #             dest_file.write(new_data)

    if has_secure_ctor_macro:
        src_template = "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\docs\\DOC_TEMPLATE_clients_secure_ctors.dox_TEMPLATE"
        destination = (
            src_template.replace("DOC_TEMPLATE_", "")
            .replace("clients_", "clients_" + modem_abbrev + "_")
            .replace(".dox_TEMPLATE", "_" + modem_abbrev + ".dox")
        )

        # open the template file
        with open(src_template, "r", encoding="UTF-8") as template_file:
            original = template_file.read()

            # make substitutions
            new_data = Template(original).substitute(sub_dict)

            # write the new file
            with open(destination, "w", encoding="UTF-8") as dest_file:
                dest_file.write(new_data)

# %%
