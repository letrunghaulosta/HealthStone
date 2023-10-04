# Setup Env
PROJECT_SOURCE_DIR=$PWD #TODO
PROJECT_BIN_DIR=$PROJECT_SOURCE_DIR/generated
TOOLCHAIN_DIR=$PROJECT_SOURCE_DIR/build
GENRATOR="Unix Makefiles"
LOGFILE=$PROJECT_SOURCE_DIR/build/build.log
LOGFILECLIENT=$PROJECT_SOURCE_DIR/build/buildClient.log
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
    ./generated/HealthStone #TODO
}

runClient()
{
    cp ./generated/HealthStone ./generated/HealthStoneClient
    clear
    ./generated/HealthStoneClient
}

help()
{
    echo -e "Usage of $0"
    echo -e "--help"
    echo -e "--clean  Clean generated"
    echo -e "--run  Run"
	echo -e "Build and run by default"
}

# clean variables
unset _CLEAN
unset _RUN
unset _CLIENT
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
        --client )
            _CLIENT=true
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
if [ "${_RUN}" == "true" ]; then
    (run)2>&1 | tee $LOGFILE
    exit 0
fi
if [ "${_CLIENT}" == "true" ]; then
    (runClient)2>&1 | tee $LOGFILECLIENT
    exit 0
fi
(
echo "===============Pre-Create================="
preCreate
echo "=================INFORM==================="
inform
echo "=================BUILD===================="
build
run
) 2>&1 | tee $LOGFILE

