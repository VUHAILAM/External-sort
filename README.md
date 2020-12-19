# External-sort
Home test Coc Coc company

Bài toán đưa ra là có một file rất lớn gồm nhiều dòng text. Chúng ta có 1 server Linux, 8GB Ram, và một bộ nhớ phần cứng không giới hạn. 
Chúng ta cần sắp xếp các dòng text theo thứ tự từ điển. Vấn đề là vì file quá lớn nên không thể sắp xếp theo các cách truyền thống được.

Về ý tưởng, thì chúng ta sẽ tải từng phần nhỏ của tập dữ liệu ban đầu đủ cho memory limit hiện có, rồi sắp xếp dữ liệu trong từng phần nhỏ đó, 
lưu lại kết quả vào các file tạm trên ổ cứng. Cuối cùng trộn các file kết quả lại để ra kết quả cuối cùng.

Cách merge thì dùng priority_queue như sau:
- đọc từng line đầu của các file con rồi đưa vào priority_queue
- lấy phần tử đầu tiên của queue ra và viết ra file output
- rồi đọc các dòng và nhét vào queue
- lặp lại cho đến khi đọc hết các file con

```sh
$ g++ main.cpp -o main
$ ./main input output memory_limit
```
