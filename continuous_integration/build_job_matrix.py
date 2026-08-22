#!/usr/bin/env python
# %%
"""
Custom matrix builder for TinyGSM step 3 - Build Matrix.

This script builds the TinyGSM-specific job matrix by:
1. Reading the modem list from TinyGsmClient.h
2. Loading the extra PlatformIO configuration
3. Applying TinyGSM-specific matrix exclusions and inclusions
4. Returning the final filtered matrix

This is designed to be called from the CI pipeline as a custom builder.
See: https://github.com/EnviroDIY/workflows
"""

import os
import re
import json

# %%
# Import helper functions from the CI pipeline
from build_utils import dict_product, remove_nested_duplicates


# %%
def get_modems_from_client_file(workspace_path: str) -> list[list[str]]:
    """
    Generate a list of currently supported modems from the client file.

    Reads TinyGsmClient.h and extracts modem defines like TINY_GSM_MODEM_SIM800
    Returns list of lists format: [["TINY_GSM_MODEM_SIM800"], ["TINY_GSM_MODEM_SIM900"], ...]
    """
    client_file = open(os.path.join(workspace_path, "src/TinyGsmClient.h"), "r")
    client_file_contents = client_file.read()
    client_file.close()

    modem_list = []
    # if defined(TINY_GSM_MODEM_SIM800)
    # NOTE: in cases where there are multiple #if defined() statements on the same line,
    # this will only find the first one. This is what we want, because we only want to
    # build one example per modem, not one example per #if defined() statement.
    pattern = re.compile(
        r"^(?:#if|#elif) defined[\s\(](?P<define>TINY_GSM_MODEM_\w+)",
        re.MULTILINE,
    )
    # find matches and add them to the lists
    for match in re.finditer(pattern, client_file_contents):
        modem_list.append([match.group("define")])

    return modem_list


# %%
def build_custom_matrix(config: dict) -> list[dict]:
    """
    Build the TinyGSM-specific job matrix.

    This is the main entry point called by the CI pipeline (3_build_matrix.py).
    """

    # %%
    # Extract config values
    workspace_path = config.get("workspace_path", os.getcwd())

    compiler_list = config.get("compiler_list", ["platformio", "arduino-cli"])
    examples_to_build = config.get("examples_to_build", [])
    build_envs = config.get("build_envs", [])
    build_fqbns = config.get("build_fqbns", [])
    boards = build_envs + build_fqbns
    inline_defines = config.get("inline_defines", [[]])
    compiler_flags = config.get("compiler_flags", [[]])

    print("=== TinyGSM Custom Matrix Builder ===")

    # Get the modem list from TinyGsmClient.h
    modem_list = get_modems_from_client_file(workspace_path)
    print(f"Found {len(modem_list)} supported modems from TinyGsmClient.h")

    # Override inline_defines with TinyGSM modems if not already set to something specific
    if inline_defines == [[]]:
        inline_defines = modem_list
        print(f"Using TinyGSM modems as inline_defines: {len(inline_defines)} modems")
    else:
        print(f"Using environment-provided inline_defines: {len(inline_defines)} flags")

    # %%
    # expand the combination of boards, modems, and examples into a job matrix

    p_cart_join = list(
        dict_product(
            {
                "compiler": ["platformio"],
                "example": examples_to_build,
                "board": build_envs,
                "inline_defines": inline_defines,
                "compiler_flags": compiler_flags,
            }
        )
    )

    a_cart_join = list(
        dict_product(
            {
                "compiler": ["arduino-cli"],
                "example": examples_to_build,
                "board": build_fqbns,
                "inline_defines": inline_defines,
                "compiler_flags": compiler_flags,
            }
        )
    )

    cart_join = p_cart_join + a_cart_join
    cart_join_list = [json.dumps(e) for e in cart_join]
    print(f"Total possible combinations: {len(cart_join)}")

    # %%
    # Exclude modems without SSL support from HttpsClient and AWS_IoTCore examples
    unsecured_modems_list = [
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
    unsecured_modems = [[modem] for modem in unsecured_modems_list]
    # secured_modems = [[modem] for modem in modem_list if [modem] not in unsecured_modems]

    small_envs = ["uno", "leonardo", "yun", "feather328p", "feather32u4", "uno_pic32"]
    small_fqbns = [
        "arduino:avr:uno",
        "arduino:avr:leonardo",
        "arduino:avr:yun",
        "adafruit:avr:feather328p",
        "adafruit:avr:feather32u4",
    ]
    small_boards = small_envs + small_fqbns

    # a list of known failures to skip in the job matrix
    matrix_exclusions = [
        {
            "compiler": compiler_list,
            "example": [os.path.join("examples", "BlynkClient")],
            "board": [
                "nano_nora",
                "arduino:esp32:nano_nora",
                "nano_every",
                "arduino:megaavr:nona4809",
                "due",
                "arduino:sam:arduino_due_x",
            ],  # these processors are not supported by the Blynk library
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [os.path.join("examples", "BlynkClient")],
            "board": boards,
            "inline_defines": [
                ["TINY_GSM_MODEM_ESP32"],
                ["TINY_GSM_MODEM_ESP8266"],
                ["TINY_GSM_MODEM_ESP8266_NONOS"],
            ],  # TinyGSM wifi modems are not supported by the Blynk library
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [os.path.join("examples", "BlynkClient")],
            "board": ["leonardo", "feather32u4", "yun"],
            "inline_defines": [["TINY_GSM_MODEM_XBEE"]],
            "compiler_flags": compiler_flags,
            # The XBee is a few bytes too big for 32u4 boards
        },
        {
            "compiler": compiler_list,
            "example": [
                os.path.join("examples", "AllFunctions"),
                os.path.join("extras", "tools", "test_build"),
            ],
            "board": small_boards,  # doesn't fit on 328p or 32u4
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [os.path.join("extras", "tools", "test_build")],
            "board": small_boards,  # doesn't fit
            "inline_defines": [["TINY_GSM_MODEM_SARAR5"]],
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [os.path.join("examples", "MqttClient")],
            "board": ["uno_pic32"],  # doesn't fit
            "inline_defines": [["TINY_GSM_MODEM_SARAR5"]],
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [os.path.join("examples", "AWS_IoTCore")],
            "board": small_boards,  # doesn't fit on 328p or 32u
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [
                os.path.join("examples", "HttpsClient"),
                os.path.join("examples", "AWS_IoTCore"),
            ],
            "board": boards,
            "inline_defines": unsecured_modems,
            "compiler_flags": compiler_flags,
            # Exclude SSL-incapable modems on SSL required examples.
        },
        {
            "compiler": compiler_list,
            "example": [
                os.path.join("extras", "tools", "AT_Debug"),
                os.path.join("extras", "tools", "AT_Spy"),
            ],  # never build these examples in CI, they are for debugging only
            "board": boards,
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": examples_to_build,
            "board": ["arduino:zephyr_main:nicla_sense"],
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [
                os.path.join("examples", "more", "Multiple_Modules"),
            ],
            "board": boards,
            "inline_defines": [
                [modem] for modem in modem_list if modem != "TINY_GSM_MODEM_ESP32"
            ],
            "compiler_flags": compiler_flags,
            # This hard-codes the modems, so only test it for that configuration
        },
    ]

    # expand the matrix exclusions
    expanded_matrix_exclusions = []
    for exclusion in matrix_exclusions:
        exclusion_list = list(dict_product(exclusion))
        expanded_matrix_exclusions.extend(exclusion_list)

    # filter out the known failures from the job matrix
    expanded_matrix_exclusions_set = remove_nested_duplicates(
        expanded_matrix_exclusions
    )
    expanded_matrix_exclusions_list = [
        json.dumps(e) for e in expanded_matrix_exclusions_set
    ]
    print(f"Matrix exclusions: {len(expanded_matrix_exclusions_list)}")

    # %%
    # eh, minimize the matrix instead of maximizing
    matrix_inclusions = [
        {
            "compiler": ["platformio"],
            "example": [os.path.join("extras", "tools", "test_build")],
            "board": [
                board
                for board in build_envs
                if board not in small_boards  # doesn't fit on 328p or 32u
            ],
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": ["arduino-cli"],
            "example": [os.path.join("extras", "tools", "test_build")],
            "board": [
                board
                for board in build_fqbns
                if board not in small_boards  # doesn't fit on 328p or 32u
            ],
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": compiler_list,
            "example": [
                os.path.join("examples", "WebClient"),
                os.path.join("examples", "MqttClient"),
            ],
            "board": small_boards,  # boards too small for the bigger test build
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": ["platformio"],
            "example": examples_to_build,
            "board": [
                "mayfly",
                "envirodiy_stonefly_m4",
                "megaatmega2560",
                "zeroUSB",
                "uno_r4_wifi",
                "nodemcu",
                "esp32dev",
            ],
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
        {
            "compiler": ["arduino-cli"],
            "example": examples_to_build,
            "board": [
                "EnviroDIY:avr:envirodiy_mayfly",
                "EnviroDIY:samd:stonefly_m4",
                "arduino:avr:mega",
                "arduino:samd:mzero_bl",
                "arduino:renesas_uno:unor4wifi",
                "esp8266:esp8266:nodemcu",
                "esp32:esp32:esp32",
            ],
            "inline_defines": modem_list,
            "compiler_flags": compiler_flags,
        },
    ]

    # expand the matrix inclusions to a list of tuples for easier filtering
    expanded_matrix_inclusions = []
    for inclusion in matrix_inclusions:
        inclusion_list = list(dict_product(inclusion))
        expanded_matrix_inclusions.extend(inclusion_list)

    # filter out the known failures from the job matrix
    expanded_matrix_inclusions_set = remove_nested_duplicates(
        expanded_matrix_inclusions
    )
    expanded_matrix_inclusions_list = [
        json.dumps(e) for e in expanded_matrix_inclusions_set
    ]
    print(f"Matrix inclusions: {len(expanded_matrix_inclusions_list)}")

    # %%
    # decide on the filtered matrix to use for the job matrix
    assembled_matrix = [
        json.loads(e)
        for e in cart_join_list
        if e not in expanded_matrix_exclusions_list
        and e in expanded_matrix_inclusions_list
    ]
    assembled_matrix = sorted(
        assembled_matrix,
        key=lambda x: (
            x["compiler"],
            x["board"],
            x["example"],
            x["inline_defines"],
            x["compiler_flags"],
        ),
    )

    for matrix_entry in assembled_matrix:
        example = matrix_entry["example"]
        example_name = os.path.split(example)[-1]
        example_full_path = os.path.join(workspace_path, example, example_name + ".ino")
        matrix_entry["other_commands"] = [
            r"sed -i 's/#define TINY_GSM_MODEM_/\/\/ #define TINY_GSM_MODEM_/g' "
            + f'"{example_full_path}"'
        ]

    final_matrix = remove_nested_duplicates(assembled_matrix)
    print(f"Final filtered matrix: {len(final_matrix)}")

    # %%
    return final_matrix


# %%
# cSpell:ignore devkitm megaatmega2560 megaavr mzero_bl renesas unor4wifi
