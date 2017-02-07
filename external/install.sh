# Create dependencies folder
mkdir dependencies
cd dependencies
DEPENDENCIES_DIR=$PWD

# params: github_url, repo_name
function install {
    echo helli
    git clone $1 $2 && \
        cd $2 && \
        mkdir build && \
        cd build && \
        cmake .. && \
        make .. && \
        make install
    cd $DEPENDENCIES_DIR 
}

install https://github.com/google/googletest.git googletest

