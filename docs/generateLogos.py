# %%
from math import floor
import os
import json
from PIL import ImageFont, Image, ImageDraw

# from IPython.display import display


# %%
# The workspace directory
docbuild_dir = "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\docs\\"
repo_name = "TinyGSM"
lib_specs_path = (
    "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\library.json"
)
logo_text = ""
short_logo_text = ""
with open(
    "C:\\Users\\sdamiano\\Documents\\GitHub\\EnviroDIY\\TinyGSM\\extras\\logo.txt", "r"
) as file:
    for line in file:
        print(line.strip())
        logo_text += line[2:]
        short_logo_text += line[18:]
    print(logo_text)
    print(short_logo_text)

print(f"Repository Name: {repo_name}")
print(f"Documentation Building Directory: {docbuild_dir}")
print("JSON Library Specs File: {}".format(lib_specs_path))

# %% parse the library info
with open(lib_specs_path) as f:
    lib_specs = json.load(f)
library_name = lib_specs["name"].replace("EnviroDIY_", "")
library_version = lib_specs["version"]

# %%
logo_sizes = {
    "gp-desktop-logo": {"width": 200, "height": 42},
    "gp-scrolling-logo": {"width": 264, "height": 55},
    "gp-mobile-logo": {"width": 132, "height": 28},
    "favicon": {"width": 32, "height": 32},
    "main_logo": {"width": 500, "height": 80},
}
ubuntu_font = f"{docbuild_dir}\\UbuntuMono-Regular.ttf"
black = (0, 0, 0, 255)
white = (255, 255, 255, 255)
ediy_green = (142, 197, 81, 255)
ediy_orange = (218, 146, 48, 255)
# ediy_header_bkgd = (34, 34, 34)
ediy_header_bkgd = (255, 255, 255, 0)  # transparent


def get_font_size(text, max_width, max_height):
    font_size = 500
    fits_in_box = False
    final_rendered_width = 0
    final_rendered_height = 0
    limiter = ""
    while fits_in_box == False:
        font = ImageFont.truetype(font=ubuntu_font, size=font_size)
        # (left, top, right, bottom)
        left, top, right, bottom = font.getbbox(text, mode="")
        rendered_width = right - left
        rendered_height = bottom - top
        # print(
        #     f"Font size: {font_size}, Rendered width: {rendered_width}, Rendered height: {rendered_height}, Max width: {max_width}, Max height: {max_height}"
        # )

        if rendered_width < max_width and rendered_height < max_height:
            fits_in_box = True
            if rendered_width < max_width and final_rendered_width > max_width:
                limiter = "width"
            if rendered_height < max_height and final_rendered_height > max_height:
                limiter = "height"
            print(
                f"Font size: {font_size}, Rendered width: {rendered_width} of {max_width}, Rendered height: {rendered_height} of {max_height}, Limiter: {limiter}"
            )
        else:
            font_size -= 1
        final_rendered_width = rendered_width
        final_rendered_height = rendered_height
    print(f"Limiter: {limiter}")
    return font_size, final_rendered_width, final_rendered_height, limiter


# %%
def create_logo(
    logo_type: str,
    library_text: str,
    version_text: str,
    add_version: bool,
    library_color,
    version_color,
):
    logo_width = logo_sizes[logo_type]["width"]
    logo_height = logo_sizes[logo_type]["height"]
    print(f"Creating {logo_type} with width: {logo_width}, height: {logo_height}")

    # calculate the line sizes
    center_x = int(logo_width / 2)
    name_lines = max(library_text.count("\n"), 1)
    n_lines = name_lines + 1 if add_version else name_lines
    line_y = floor(logo_height / n_lines)
    extra_y = logo_height - (line_y * n_lines)
    # if the logo is only one line long, pad it with the extra space,
    first_line_y = line_y + extra_y if name_lines == 1 else line_y
    # otherwise, add the extra space to the last line
    last_line_y = line_y + extra_y if name_lines > 1 else line_y

    # calculate the max font size for the longest line of the library name / non-version text
    longest_line = max(library_text.splitlines(), key=len)
    font_size, _, final_rendered_height, limiter = get_font_size(
        longest_line, logo_width, first_line_y
    )

    # if the limiting factor is width, recalculate how tall the name will be and then give the extra space to the last line
    if limiter == "width":
        line_y = final_rendered_height
        extra_y = logo_height - (line_y * n_lines)
        if not add_version:
            first_line_y = line_y + floor(extra_y / 2)
        else:
            first_line_y = line_y
        last_line_y = line_y + extra_y

    # create a new image with a black background
    img = Image.new(mode="RGBA", size=(logo_width, logo_height), color=ediy_header_bkgd)
    # prepare to draw on the image
    draw = ImageDraw.Draw(img)
    # set the font for drawing text
    # font = ImageFont.truetype(<font-file>, <font-size>)
    font = ImageFont.truetype(font=ubuntu_font, size=font_size)
    # add the library name
    for lineNum, line in enumerate(library_text.splitlines()):
        y_pos = first_line_y + (line_y * lineNum)
        print(
            f"Adding line {lineNum}: {line} to {logo_type} centered at {center_x}, anchored at {y_pos}"
        )
        draw.text(
            xy=(center_x, y_pos),  # set the baseline in the center
            text=line,
            fill=library_color,
            font=font,
            spacing=0,
            anchor="mb",  # anchor x to the center an y to the bottom
            # (bottom of the lowest letters)
            # NOTE: not descender
        )
    if add_version:
        # recalculate the font size for the version text, setting the max height to the last line height minus 3 pixels of padding
        font_size_version = get_font_size(version_text, logo_width, last_line_y - 3)[0]
        # add the library version
        print(
            f"Adding version: {version_text} to {logo_type} centered at {center_x}, anchored at {logo_height}"
        )
        draw.text(
            xy=(center_x, logo_height),  # set the baseline in the center
            text=version_text,
            fill=version_color,
            font=ImageFont.truetype(font=ubuntu_font, size=font_size_version),
            spacing=0,
            anchor="mb",  # anchor x to the center an y to the bottom
            # (top of the tallest letters)
            # NOTE: not ascender
        )
    # display(img)
    img.save(f"{docbuild_dir}\\{logo_type}.png")
    print(f"Saved {docbuild_dir}\\{logo_type}.png")


# %%
for logo_size in logo_sizes.keys():
    print(f"Creating logo of size: {logo_size}")
    if "gp" in logo_size:
        create_logo(
            logo_type=logo_size,
            library_text=logo_text,
            version_text=library_version,
            add_version=True,
            library_color=white,
            version_color=white,
        )
    elif logo_size == "favicon":
        create_logo(
            logo_type=logo_size,
            library_text=short_logo_text,
            version_text=library_version,
            add_version=False,
            library_color=white,
            version_color=white,
        )
    elif logo_size == "main_logo":
        create_logo(
            logo_type=logo_size,
            library_text=logo_text,
            version_text=library_version,
            add_version=False,
            library_color=black,
            version_color=black,
        )

# %%
# cSpell:words ediy bkgd getbbox
