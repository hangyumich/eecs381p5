./test <../test/errors_in.txt >output.txt
diff ../test/errors_out.txt output.txt
./test <../test/show_in.txt >output.txt
diff ../test/show_out.txt output.txt
./test <../test/status_in.txt >output.txt
diff ../test/status_out.txt output.txt
./test <../test/view_in.txt >output.txt
diff ../test/view_out.txt output.txt
