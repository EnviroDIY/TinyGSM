#!/usr/bin/env python
# %%
from copy import deepcopy
from itertools import chain
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
acli_skip_boards = ["uno_pic32", "genuino101", "bluepill_f103c8"]

# %%
# set verbose
use_verbose = False
if "RUNNER_DEBUG" in os.environ.keys() and os.environ["RUNNER_DEBUG"] == "1":
    use_verbose = True


# %%
# Some working directories

# The workspace directory
if "GITHUB_WORKSPACE" in os.environ.keys():
    workspace_dir = os.environ.get("GITHUB_WORKSPACE", os.getcwd())
else:
    workspace_dir = os.getcwd()
if "\\continuous_integration" in workspace_dir:
    workspace_dir = workspace_dir.replace("\\continuous_integration", "")
workspace_path = os.path.abspath(os.path.realpath(workspace_dir))
print(f"Workspace Path: {workspace_path}")


# %%
# The examples directory
examples_dir = "./examples/"
examples_path = os.path.join(workspace_dir, examples_dir)
examples_path = os.path.abspath(os.path.realpath(examples_path))
print(f"Examples Path: {examples_path}")

# The extras directory
extras_dir = "./extras/"
extras_path = os.path.join(workspace_dir, extras_dir)
extras_path = os.path.abspath(os.path.realpath(extras_path))
print(f"Extras Path: {extras_path}")

# The continuous integration directory
ci_dir = "./continuous_integration/"
ci_path = os.path.join(workspace_dir, ci_dir)
ci_path = os.path.abspath(os.path.realpath(ci_path))
print(f"Continuous Integration Path: {ci_path}")
if not os.path.exists(ci_path):
    print(f"Creating the directory for CI: {ci_path}")
    os.makedirs(ci_path, exist_ok=True)

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
# Generate a list of currently supported modems from the client file
client_file = open(os.path.join(workspace_path, "src/TinyGsmClient.h"), "r")
client_file_contents = client_file.read()
client_file.close()

modem_list = []
# if defined(TINY_GSM_MODEM_SIM800)
pattern = re.compile(
    r"^(?:#if|#elif) defined[\s\(](?P<define>TINY_GSM_MODEM_\w+)",
    re.MULTILINE,
)
# find matches and add them to the lists
for match in re.finditer(pattern, client_file_contents):
    # print(match.group("define"))
    modem_list.append(match.group("define"))


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
# read configurations based on existing files and environment variables

# Arduino CLI configuration
# Always use the generic one from the shared workflow repository
if "GITHUB_WORKSPACE" in os.environ.keys():
    arduino_cli_config = os.path.join(ci_path, "arduino_cli.yaml")
    if not os.path.isfile(arduino_cli_config):
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
    arduino_cli_config = os.path.join(ci_dir, "arduino_cli_local.yaml")

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
        print(f"::debug::{os.environ.get("BOARDS_TO_BUILD")}")
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

# convert the list of boards into list of FQBNs, PIO environments, and PIO bare boards
fqbns_to_build = [
    pio_to_acli[board]["fqbn"]
    for board in boards
    if board in pio_to_acli.keys() and not board in acli_skip_boards
]
pio_envs_to_build = [
    env
    for env in pio_config.envs()
    if pio_config.get("env:{}".format(env), "board") in boards
    and not pio_config.get("env:{}".format(env), "board") in pio_skip_boards
]
pio_bare_boards = [
    board
    for board in boards
    if board not in board_to_pio_env.keys() and not board in pio_skip_boards
]

# print out what will be built
if use_verbose:
    print("::debug::==========================================================")
    print("::debug::Building the following Arduino FQBNs:")
    for board in boards:
        print(
            f"::debug::Requested Board: {board} -- FQBN: {pio_to_acli[board]['fqbn']}"
        )
    print("::debug::==========================================================")

    print("::debug::Building the following PlatformIO environments and boards:")
    for env in pio_envs_to_build:
        print(
            f"::debug::Requested Board: {pio_env_to_board[env]} -- Environment Name: {env} -- platformio.ini source: {'workflow default' if default_pio_config_file else 'repository CI directory'}"
        )
    for board in pio_bare_boards:
        print(f"::debug::Requested Board: {board} -- NO ENVIRONMENT CONFIGURATION")
    print("::debug::==========================================================")

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
        print(f"\nSearching for examples in {root}({os.path.split(root)[
                -1
            ]})\n\t{subdirs}\n\t\t{files}")
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
                print(f"\t- example: {filename} (full path: {file_path})")
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
            e.lower() in os.path.normpath(example).split(os.sep)
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
# helper functions to create commands
def create_arduino_cli_compile_command(
    code_subfolder: str,
    fqbn: str,
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
        "text",
        "--fqbn",
        fqbn,
    ]
    arduino_command_args += [
        f'"{os.path.join(workspace_path, code_subfolder)}"',
    ]
    return " ".join(arduino_command_args)


def create_pio_ci_compile_command(
    code_subfolder: str,
    pio_board_or_env: str,
    use_pio_config_file: bool,
) -> str:
    pio_command_args = [
        "pio",
        "ci",
    ]
    if use_verbose:
        pio_command_args += ["--verbose"]
    if use_pio_config_file:
        pio_command_args += [
            "--project-conf",
            f'"{pio_config_file}"',
            "--environment",
            pio_board_or_env,
        ]
    else:
        pio_command_args += [
            "--board",
            pio_board_or_env,
        ]
    pio_command_args += [
        f'"{os.path.join(workspace_path, code_subfolder)}"',
    ]
    return " ".join(pio_command_args)


def create_multi_env_pio_ci_compile_command(
    code_subfolder: str,
    pio_board_or_env_list: List[str],
    use_pio_config_file: bool,
) -> str:
    pio_command_args = [
        "pio",
        "ci",
    ]
    if use_verbose:
        pio_command_args += ["--verbose"]
    if use_pio_config_file:
        pio_command_args += [
            "--project-conf",
            f'"{pio_config_file}"',
        ]
        for pio_board_or_env in pio_board_or_env_list:
            pio_command_args += [
                "--environment",
                pio_board_or_env,
            ]
    else:
        for pio_board_or_env in pio_board_or_env_list:
            pio_command_args += [
                "--board",
                pio_board_or_env,
            ]
    pio_command_args += [
        f'"{os.path.join(workspace_path, code_subfolder)}"',
    ]
    return " ".join(pio_command_args)


def add_log_to_compile_command(command: str, group_title: str) -> List:
    command_list = []
    command_list.append("\necho ::group::{}".format(group_title))
    command_list.append(command + " 2>&1 | tee output.log")
    command_list.append("result_code=${PIPESTATUS[0]}")
    command_list.append(
        'if [ "$result_code" -eq "0" ]; then echo -e " - {title} :white_check_mark:" >> $GITHUB_STEP_SUMMARY; else echo -e " - {title} :x:" >> $GITHUB_STEP_SUMMARY; fi'.format(
            title=group_title
        )
    )
    command_list.append(
        'if [ "$result_code" -eq "0" ] && [ "$status" -eq "0" ]; then status=0; else status=1; fi'
    )
    command_list.append("echo ::endgroup::")
    command_list.append(
        'if [ "$result_code" -eq "0" ]; then echo -e "\\e[32m{title} successfully compiled\\e[0m"; else echo -e "\\e[31m{title} failed to compile\\e[0m"; fi'.format(
            title=group_title
        )
    )
    return command_list


# %%
# set up outputs
arduino_job_matrix = []
pio_job_matrix = []
start_job_commands = "status=0"
end_job_commands = "\n\nexit $status"

# %%
# Create job info for the basic examples
# Use one job per board with one command per example
for example in examples_to_build:
    start_commands = [
        start_job_commands,
        f"sed -i 's/#define TINY_GSM_MODEM_/\\/\\/ #define TINY_GSM_MODEM_/g' \"{os.path.join(workspace_path,example,os.path.split(example)[-1]+'.ino')}\"",
        # f"sed -i 's/\\/\\/ #pragma/#pragma/g' \"{os.path.join(workspace_path, 'src', 'TinyGsmClient.h')}\"",
    ]

    arduino_ex_commands = deepcopy(start_commands)
    pio_ex_commands = deepcopy(start_commands)
    for modem in modem_list:
        sed_addition = f"sed -i '1i\\\n#define {modem}\\\n' \"{os.path.join(workspace_path,example,os.path.split(example)[-1]+'.ino')}\""

        # create commands for the Arduino CLI
        # can only specify FQBN, so each board can only be built one way
        for fqbn in fqbns_to_build:
            build_command = create_arduino_cli_compile_command(
                code_subfolder=example,
                fqbn=fqbn,
            )
            command_with_log = add_log_to_compile_command(
                command=sed_addition + "\n" + build_command,
                group_title=f"{fqbn} - {modem}",
            )
            arduino_ex_commands.extend(command_with_log)

        # create commands for PlatformIO
        # use the environments list to catch all environments - even those using the same board
        for env in pio_envs_to_build:
            build_command = create_pio_ci_compile_command(
                code_subfolder=example,
                pio_board_or_env=env,
                use_pio_config_file=True,
            )
            command_with_log = add_log_to_compile_command(
                command=sed_addition + "\n" + build_command,
                group_title=f"{env} - {modem}",
            )
            pio_ex_commands.extend(command_with_log)
        # use the bare board list to catch boards requested in the inputs but not in the platformio.ini file
        for pio_board in pio_bare_boards:
            build_command = create_pio_ci_compile_command(
                code_subfolder=example,
                pio_board_or_env=pio_board,
                use_pio_config_file=False,
            )
            command_with_log = add_log_to_compile_command(
                command=sed_addition + "\n" + build_command,
                group_title=f"{pio_board} - {modem}",
            )
            pio_ex_commands.extend(command_with_log)

    arduino_job_matrix.append(
        {
            "job_name": "Arduino - {}".format(os.path.split(example)[-1]),
            "job_tag": os.path.split(example)[-1].lower().replace(" ", "-"),
            "command": "\n".join(arduino_ex_commands + [end_job_commands]),
        }
    )
    pio_job_matrix.append(
        {
            "job_name": "PlatformIO - {}".format(os.path.split(example)[-1]),
            "job_tag": os.path.split(example)[-1].lower().replace(" ", "-"),
            "command": "\n".join(pio_ex_commands + [end_job_commands]),
        }
    )


# %%
# Convert commands in the matrix into bash scripts
for matrix_job in arduino_job_matrix + pio_job_matrix:
    bash_file_name = matrix_job["job_name"].replace(" ", "") + ".sh"
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
    bash_out.write(matrix_job["command"])
    bash_out.close()
    matrix_job["script"] = os.path.join(artifact_path, bash_file_name)

# Remove the command from the dictionaries before outputting them
for items in arduino_job_matrix + pio_job_matrix:
    if "command" in items:
        del items["command"]


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
# cSpell:words devkitm acli genuino bluepill fqbn fqbns pipestatus jsons endgroup
