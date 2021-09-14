#find ./ -iname *.h -o -iname *.cpp -o -iname *.hpp | xargs clang-format-6.0 -style=file -i
find . -regex '.*\.\(cpp\|hpp\|c\|h\)' -exec clang-format -style=file -i {} \;

find . -name "*.c" -o -name "*.h" -o -name "*.cc" -o -name "*.hh" -o -name "*.cpp" -o -name "*.hpp" -o -name "*.py" -type f | grep -v gtest | gtags -f - -iv
