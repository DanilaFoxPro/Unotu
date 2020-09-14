@echo off
call cloc .\code .\data --exclude-dir=_dependencies --exclude-list-file=CodeStatistics_SkipFiles.txt
pause