import re


def bump_version(file_path, pattern, new_version):
    with open(file_path, "r") as file:
        content = file.read()

    new_content = re.sub(pattern, new_version, content)

    with open(file_path, "w") as file:
        file.write(new_content)


def main(new_version):
    # Define file paths
    python_file = "raspa/utils.py"
    pyproject_file = "pyproject.toml"
    cmake_file = "CMakeLists.txt"
    pkgbuild_file = "packaging/PKGBUILD"

    # Define patterns to match the current version
    python_pattern = r'RASPA_VERSION\s*=\s*"\d+\.\d+\.\d+"'
    pyproject_pattern = r'version\s*=\s*"\d+\.\d+\.\d+"'
    cmake_pattern = r"VERSION \d+\.\d+\.\d+"
    pkgbuild_pattern = r"pkgver=\d+\.\d+\.\d+"

    # Define new version strings
    new_python_version = f'RASPA_VERSION = "{new_version}"'
    new_pyproject_version = f'version = "{new_version}"'
    new_cmake_version = f"VERSION {new_version}"
    new_pkgbuild_version = f"pkgver={new_version}"

    # Bump versions in respective files
    bump_version(python_file, python_pattern, new_python_version)
    bump_version(pyproject_file, pyproject_pattern, new_pyproject_version)
    bump_version(cmake_file, cmake_pattern, new_cmake_version)
    bump_version(pkgbuild_file, pkgbuild_pattern, new_pkgbuild_version)


if __name__ == "__main__":
    import sys

    if len(sys.argv) != 2:
        print("Usage: python bump_version.py <new_version>")
    else:
        main(sys.argv[1])
