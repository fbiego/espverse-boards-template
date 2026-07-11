from pathlib import Path

Import("env")


project_dir = Path(env.subst("$PROJECT_DIR"))
project_headers = [
    str(path)
    for pattern in ("*.h", "*.hpp")
    for path in (project_dir / "include").rglob(pattern)
]

if project_headers:
    build_dir = env.subst("$BUILD_DIR")
    source_names = (
        "main.cpp",
        "board/board_profile.cpp",
        "display/lvgl_port.cpp",
        "network/requests.cpp",
        "os/lv_os_custom.cpp",
    )
    for source_name in source_names:
        env.Depends(str(Path(build_dir) / "src" / f"{source_name}.o"),
                    project_headers)
