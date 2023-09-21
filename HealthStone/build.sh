# Setup Env
PROJECT_SOURCE_DIR=$(cygpath -m $PWD)
PROJECT_BIN_DIR=$PROJECT_SOURCE_DIR/generated
TOOLCHAIN_DIR=$PROJECT_SOURCE_DIR/build
GENRATOR="Unix Makefiles"
LOGFILE=$PROJECT_SOURCE_DIR/build/build.log

# Function definition
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
    echo "Generating CMake..."
    cmake -G"${GENRATOR}" -H"${PROJECT_SOURCE_DIR}" -B"${PROJECT_BIN_DIR}" -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_DIR}/toolchain.cmake"
    echo "Compiling..."
    make -C generated
}

clean()
{
    echo "Deleting generated..."
    rm -rf generated
}

run()
{
    clear
    ./generated/HealthStone.exe
}

help()
{
    echo -e "Usage of $0"
    echo -e "--help"
    echo -e "--clean  Clean generated"
    echo -e "--run  Build and Run"
}

# clean variables
unset _CLEAN
unset _RUN
unset _HELP

# Parse input
while [[ $# -gt 0 ]]
do
    key=$1
    case $key in
        --help )
            _HELP=true
            shift
            ;;
        --clean )
            _CLEAN=true
            shift
            ;;
        --run )
            _RUN=true
            shift
            ;;
        * )
            echo "Invalid option: $1"
            help
            exit 1
            ;;
    esac
done

if [ "${_HELP}" == "true" ]; then
    echo "=================Usage==================="
    help
    exit 0
fi

if [ "${_CLEAN}" == "true" ]; then
    echo "=================CLEAN==================="
    clean
    exit 0
fi

(
echo "===============Pre-Create================="
preCreate
echo "=================INFORM==================="
inform
echo "=================BUILD===================="
build
) 2>&1 | tee $LOGFILE

if [ "${_RUN}" == "true" ]; then
    run
fi