#!/usr/bin/env python3
import os
import sys
import json
import shutil
import subprocess
import argparse
from pathlib import Path

# Metadata for the bootstrap tool
__TOOL_VERSION__ = "0.1.0-alpha"
__PLATFORM_NAME__ = "ExtraC (XC) Development Platform"

class ExtraCProject:
    def __init__(self):
        self.root = self._find_project_root()
        self.xc_dir = self.root / ".xc" if self.root else Path(".xc")
        self.config_path = self.xc_dir / "config.json"
        # Respect environment variable for global Apps directory
        self.apps_dir = Path(os.getenv("XC_APPS_PATH", "/usr/local/share/xc/apps"))
        self.config = self._load_config()

    def _find_project_root(self):
        curr = Path.cwd()
        while curr != curr.parent:
            if (curr / ".xc").exists():
                return curr
            curr = curr.parent
        return None

    def _load_config(self):
        if self.config_path.exists():
            with open(self.config_path, 'r') as f:
                return json.load(f)
        return {
            "project_name": "unnamed_project",
            "distributor_domain": "org.extrac",
            "version": "0.1.0",
            "compiler_flags": ["-fPIC", "-shared", "-Wall", "-Wextra"],
            "include_dirs": ["lib", "src"]
        }

    def save_config(self):
        self.xc_dir.mkdir(exist_ok=True)
        with open(self.config_path, 'w') as f:
            json.dump(self.config, f, indent=4)

    def _get_module_output_name(self, module_path):
        # Swap slashes for dots: net/http -> libnet.http.so
        dotted_name = module_path.replace(os.sep, '.')
        return f"lib{dotted_name}.so"

    def build_module(self, module_rel_path):
        src_dir = self.root / "src" / module_rel_path
        if not src_dir.exists():
            print(f"Error: Module source directory {src_dir} not found.")
            return

        build_cache = self.xc_dir / "build" / module_rel_path
        build_cache.mkdir(parents=True, exist_ok=True)

        sources = []
        build_json = src_dir / "build.json"
        
        # Priority 1: build.json
        if build_json.exists():
            with open(build_json, 'r') as f:
                data = json.load(f)
                sources = [str(src_dir / s) for s in data.get("sources", [])]
        # Priority 2: //SOURCE tag scanning
        else:
            for root, _, files in os.walk(src_dir):
                for file in files:
                    if file.endswith((".xc", ".c")):
                        full_path = Path(root) / file
                        try:
                            with open(full_path, 'r', encoding='utf-8') as f:
                                first_line = f.readline()
                                if first_line.strip().startswith("//SOURCE"):
                                    sources.append(str(full_path))
                        except (IOError, UnicodeDecodeError):
                            continue

        if not sources:
            print(f"Skipping {module_rel_path}: No valid source files identified.")
            return

        output_name = self._get_module_output_name(module_rel_path)
        output_path = build_cache / output_name

        # Invoke underlying C compiler (GCC/Clang)
        cmd = ["gcc"] + self.config["compiler_flags"]
        for inc in self.config["include_dirs"]:
            cmd += ["-I", str(self.root / inc)]
        cmd += ["-o", str(output_path)] + sources

        print(f"Building: {module_rel_path} -> {output_path}")
        result = subprocess.run(cmd)
        if result.returncode == 0:
            print(f"Build successful.")
            return output_path
        else:
            print(f"Build failed with exit code {result.returncode}.")
            return None

    def install(self):
        domain = self.config["distributor_domain"]
        name = self.config["project_name"]

        # Handle lib/ directory (filtering __ internals)
        lib_target = self.apps_dir / domain / "libs"
        lib_target.mkdir(parents=True, exist_ok=True)
        src_lib = self.root / "lib"
        
        if src_lib.exists():
            for entry in src_lib.iterdir():
                if not entry.name.startswith("__"):
                    dest = lib_target / entry.name
                    if entry.is_dir():
                        shutil.copytree(entry, dest, dirs_exist_ok=True)
                    else:
                        shutil.copy2(entry, dest)

        # Handle built modules
        mod_target = self.apps_dir / domain / name
        mod_target.mkdir(parents=True, exist_ok=True)
        build_root = self.xc_dir / "build"
        
        if build_root.exists():
            for root, _, files in os.walk(build_root):
                for file in files:
                    if file.endswith(".so"):
                        shutil.copy2(Path(root) / file, mod_target / file)
        
        print(f"Installed project '{name}' to {self.apps_dir}")

def main():
    parser = argparse.ArgumentParser(
        prog="xc",
        description=f"{__PLATFORM_NAME__} Bootstrap Tool",
        add_help=True
    )
    parser.add_argument("-v", "--version", action="version", version=f"%(prog)s {__TOOL_VERSION__}")
    
    subparsers = parser.add_subparsers(dest="command", help="Available commands")

    # init
    subparsers.add_parser("init", help="Initialize a new XC project structure")

    # build
    build_p = subparsers.add_parser("build", help="Compile XC modules")
    build_p.add_argument("modules", nargs="*", help="Paths relative to src/ to build as modules")
    build_p.add_argument("--standalone", action="store_true", help="Compile as standalone executable (WIP)")

    # install / uninstall
    subparsers.add_parser("install", help="Install project to the global Apps directory")
    subparsers.add_parser("uninstall", help="Remove project from the global Apps directory (WIP)")

    # run
    subparsers.add_parser("run", help="Execute an app or the current project (WIP)")

    # info
    info_p = subparsers.add_parser("info", help="Manage project metadata and environment configuration")
    info_sub = info_p.add_subparsers(dest="subcommand")
    
    set_p = info_sub.add_parser("set", help="Set a configuration key")
    set_p.add_argument("key", help="Config key (e.g., distributor_domain)")
    set_p.add_argument("value", help="Value to assign")
    
    del_p = info_sub.add_parser("delete", help="Remove a configuration key")
    del_p.add_argument("key", help="Key to delete")

    # Default to help if no arguments
    if len(sys.argv) == 1:
        parser.print_help(sys.stderr)
        sys.exit(0)

    args = parser.parse_args()
    ctx = ExtraCProject()

    if args.command == "init":
        ctx.save_config()
        (Path.cwd() / "src").mkdir(exist_ok=True)
        (Path.cwd() / "lib").mkdir(exist_ok=True)
        print(f"Initialized XC environment in {Path.cwd()}")

    elif args.command == "build":
        if args.standalone:
            print("Notice: Standalone executable compilation is a Work-In-Progress. Please check back later.")
        elif not args.modules:
            print("Error: No modules specified for build.")
        else:
            for mod in args.modules:
                ctx.build_module(mod)

    elif args.command == "info":
        if args.subcommand == "set":
            ctx.config[args.key] = args.value
            ctx.save_config()
        elif args.subcommand == "delete":
            ctx.config.pop(args.key, None)
            ctx.save_config()
        print(json.dumps(ctx.config, indent=2))

    elif args.command == "install":
        ctx.install()

    elif args.command in ["uninstall", "run"]:
        print(f"Notice: The '{args.command}' command is a Work-In-Progress. Please check back later.")

if __name__ == "__main__":
    main()
