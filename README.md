# WordWrap
Caitlyn Romano - cer161
Anthony Romanushko - ar1738

Testing strategy:
Our overall testing strategy was trying to break our program by inputing numerous file instances and then altering our code from there. We tested for the program to work for all buffer sizes by creating a macro for buf_size and altering its size from 1-1000. We also ensured that the program would work if a word exceeded the width. We played around with different file inputs such as files with a lot of blank lines and spaces to ensure we were getting the proper output.
We tested part two by generating many directories that included several files with text that included the various cases that would trigger word wrapping and also included the exceptions such as words that are longer than the wrap size.
We also tested on directories multiple times to make sure that already wrapped files would not be wrapped again.
