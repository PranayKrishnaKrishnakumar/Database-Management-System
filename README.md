# Database Management System
Clone the git repo and run following command to build and run the project. Once the build command is run, it will create a binary names `assignment`. Here are the commands,

    cd assign1_storage_manager # enter the first assignment directory
    make                       # to build the project
    ./assignment               # to run the project

We have added three additional tests so make sure that few of the functionalities are working fine.

`testCurrentAndTotalPageValue()` : Try to add more file blocks and check if the total pages are reflecting correctly.

`testTotalNumPagesOfOpenedFile()` : Check if the `totalNumPages` variable is correctly updated when a file with non zero size is opened.

`testEnsureCapacity()` : Check if the `totalNumPages` is updated correctly after running the `ensureCapacity` function.
