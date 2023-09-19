PROJECT_SOURCE_DIR="C:/Users/LHA9HC/Desktop/Storage/DarkData/Cpp_Training/Practice/HealthStone"
PROJECT_BIN_DIR=$PROJECT_SOURCE_DIR/generated
TOOLCHAIN_DIR=$PROJECT_SOURCE_DIR/build
GENRATOR="Unix Makefiles"
preCreate()
{
    echo "Creating bin dir..."
    mkdir -p generated
}

inform()
{
    echo "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}"
    echo "PROJECT_BIN_DIR: ${PROJECT_BIN_DIR}"
    echo "TOOLCHAIN_DIR: ${TOOLCHAIN_DIR}"
}

build()
{
    cmake -G"${GENRATOR}" -H"${PROJECT_SOURCE_DIR}" -B"${PROJECT_BIN_DIR}" -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_DIR}/toolchain.cmake"
}

echo "===============Pre-Create================="
preCreate
echo "=================INFORM==================="
inform
echo "=================BUILD===================="
build
