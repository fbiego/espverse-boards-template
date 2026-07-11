# ESPVerse Board Template Picker Page

Build a page in the ESPVerse website repo that lets users download a trimmed
copy of `espverse-boards-template` for only the boards they select.

## Goal

The source template repo contains support for many PlatformIO boards. Users
should not need to download every board profile and every PlatformIO environment
when they only want one or two boards.

The website page should:

- Show all supported board environments from the template repo.
- Let users select one or more boards.
- Download the template repository as a zip.
- Generate a new zip in the browser that keeps only the selected board support.
- Preserve the website's existing design system, layout, typography, and
  components.

## Source Repository

Use the GitHub repository that hosts this template:

```text
fbiego/espverse-boards-template
```

If the website has configuration for GitHub org/repo names, use that instead of
hard-coding where practical.

Default branch:

```text
main
```

The page needs to fetch:

```text
https://raw.githubusercontent.com/fbiego/espverse-boards-template/main/platformio.ini
https://github.com/fbiego/espverse-boards-template/archive/refs/heads/main.zip
```

## Board Discovery

Parse `platformio.ini` and find every section whose name starts with:

```ini
[env:
```

Each `[env:*]` section is one selectable board environment.

For each environment, extract:

- Environment id: from `[env:the-id]`
- Display name: preferably the nearest preceding comment line, for example
  `; M5 StopWatch`
- Board profile header: from the build flag:

```ini
-D BOARD_PROFILE_INCLUDE=\"boards/m5-stopwatch.hpp\"
```

Some environments intentionally share the same board profile header. For example,
multiple M5 Core S3 variants can point to the same `include/boards/m5-core-s3.hpp`
file. The trimming logic must keep headers based on the selected environments'
actual `BOARD_PROFILE_INCLUDE` value.

## UI Requirements

Create a page that feels native to the ESPVerse website.

Minimum controls:

- Board search/filter field.
- Board selection list with checkboxes.
- Select all button.
- Clear selection button.
- Download button.
- Loading and error states.

Each board row/card should show:

- Human readable board name.
- PlatformIO environment id.
- Board profile header filename.

The download button should be disabled until at least one board is selected.

## Zip Generation

Use client-side zip generation so the website does not need a backend. JSZip is
fine unless the website already has a preferred zip library.

Install or load:

```text
jszip
```

Download the full repository zip from GitHub, then create a new zip with filtered
contents.

## Files To Remove From Generated Zip

Always exclude these paths from the generated zip:

```text
.git/
.pio/
.vscode/
docs/
firmware/
```

Also exclude all unselected board profile headers from:

```text
include/boards/
```

Keep these shared board files even if they are not selected directly:

```text
include/boards/common.hpp
include/boards/default.hpp
```

Keep the rest of the project structure:

```text
src/
include/display/
include/displays/
include/os/
include/board/
lib/
partitions/
scripts/
test/
platformio.ini
.gitignore
```

## PlatformIO Trimming

The generated `platformio.ini` should be rewritten.

Keep these shared sections:

```ini
[platformio]
[env]
[esp32]
```

Keep only selected board environment sections:

```ini
[env:selected-board]
```

Remove all unselected `[env:*]` sections.

The selected sections must remain valid and should preserve their original
content, comments, `lib_deps`, `build_flags`, partition config, and board config.

## Suggested Parser Shape

In JavaScript:

1. Normalize line endings to `\n`.
2. Split `platformio.ini` into section blocks.
3. A section starts at a line matching:

```js
/^\s*\[([^\]]+)\]\s*$/
```

4. Shared blocks are those named:

```js
platformio
env
esp32
```

5. Board env blocks are those named like:

```js
env:m5-stopwatch
```

6. Header extraction regex can be:

```js
/BOARD_PROFILE_INCLUDE=\\?"boards\/([^"\\]+\.hpp)\\?"/
```

7. Display title can use the closest non-empty preceding comment line before the
   section:

```ini
; M5 StopWatch
[env:m5-stopwatch]
```

## Download Filename

Use a filename like:

```text
espverse-template-m5-stopwatch.zip
espverse-template-custom.zip
```

For multiple boards, either join selected ids or use `custom` if the filename
would become too long.

## Acceptance Criteria

- The page discovers all current board environments from `platformio.ini`.
- Selecting `m5-stopwatch` keeps:

```text
include/boards/m5-stopwatch.hpp
include/boards/common.hpp
include/boards/default.hpp
```

- Selecting `m5-stopwatch` removes unrelated board headers such as:

```text
include/boards/viewe-s3-15.hpp
include/boards/waveshare-s3-lcd-128.hpp
```

- The generated `platformio.ini` keeps `[platformio]`, `[env]`, `[esp32]`, and
  `[env:m5-stopwatch]`.
- The generated `platformio.ini` removes unselected `[env:*]` sections.
- The generated zip can be opened as a PlatformIO project.
- The implementation has clear loading, error, and success feedback.

## Notes

This should be implemented inside the ESPVerse website repo rather than in this
template repo, so the page can reuse the website's routing, components, branding,
and deployment setup.
