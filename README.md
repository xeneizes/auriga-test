# Task 1.
### 1) The following code compiles on Linux. It has a number of problems, however. Please locate as many of those problems as you are able and provide your recommendations regarding how they can be resolved.

```
typedef struct list_s
{
struct list_s *next; /* NULL for the last item in a list */
int data;
}
list_t;
/* Counts the number of items in a list.
*/
int count_list_items(const list_t *head) {
if (head->next) {
return count_list_items(head->next) + 1;
} else {
return 1;
}
}
/* Inserts a new list item after the one specified as the argument.
*/
void insert_next_to_list(list_t *item, int data) {
(item->next = malloc(sizeof(list_t)))->next = item->next;
item->next->data = data;
}
/* Removes an item following the one specificed as the argument.
*/
void remove_next_from_list(list_t *item) {
if (item->next) {
free(item->next);
item->next = item->next->next;
}
}
/* Returns item data as text.
*/
char *item_data(const list_t *list)
{
char buf[12];
sprintf(buf, "%d", list->data);
return buf;
}
```

### Answer

* in function ```int count_list_items(const list_t *head)``` — it’s better not to call itself recursivly and just jump over the each list member until data is NULL. Cause counting big lists’ size would be very expensive 
* in function ```void insert_next_to_list(list_t *item, int data)``` – not sure but I think item->next pointer will be lost before it will be stored at new element. I would rewrite it like this to make code more clear:
```
list_t *tmp = malloc(sizeof(list_t));
tmp->data = data;
tmp->next = item->next;
item->next = tmp;
```
also ```item``` and ```data``` argument should be const
* in function ```void remove_next_from_list(list_t *item)``` –  the next element will be freed first, and its pointer is used after that. That is not right. First we need to remap next and only after that delete an element:
```
if (item->next) {
list_t *tmp = item->next;
item->next = item->next->next;
free(tmp);
}
```
also argument ```item``` should be const

* in function ```*item_data(const list_t *list)``` local ```buf``` array will be destroyed after exiting the function. We may use STL ```std::string``` container (or ```std::string_view```) as the returning type of the function, or use ```char``` pointer as the second argument to print text value there.
Also if the integer is 64-bit, 12-bytes buf would be not enough to print too big number. 
Also ```snprintf``` should be used instead of ```sprintf```


# Task 2.

### 2) By default, Linux builds user-space programs as dynamically linked applications. Please describe scenarios where you would change the default behavior to compile your application as a statically linked one.

### Answer

I think static linking may be used is such cases as
* we should guarantee the running of the software at different user environments
* it’s used at limited environments such as simple MCUs, linux boot and etc.
* to speed-up software
* to protect software from vulnerabilities (intruder cannot substitute libraries an easy way)

# Task 3.
### 3) Develop your version of the ls utility. Only the 'ls -l' functionality is needed.
### Answer
See ```myls``` folder.

# Task 4.
### 4) Please explain, at a very high-level, how a Unix OS, such as Linux, implements the break-point feature in a debugger.
### Answer
For debugging in Linux ptrace API is used.
TBD

# Task 5.
### 5) Suppose you debug a Linux application and put a break-point into a function defined by a dynamically-linked library used by the application. Will that break-point stop any other application that makes use of the same library?
### Answer
TBD

# Task 6.
### 6) Please translate in English. Don’t metaphrase, just put the idea as you see it into English.
```Если пользовательский процесс начал операцию ввода-вывода с устройством, и остановился, ожидая окончания операции в устройстве, то операция в устройстве закончится во время работы какого-то другого процесса в системе. Аппаратное прерывание о завершении операции будет обработано операционной системой. Так как на обработку прерывания требуется некоторое время, то текущий процесс в однопроцессорной системе будет приостановлен. Таким образом, любой процесс в системе непредсказуемо влияет на производительность других процессов независимо от их приоритетов.```
### Answer
```If user process started the IO-operation with the device and then stopped, waiting for the end of operation in it, then operation in the device would end during the another processs working in the system. End of operation hardware interrupt would be handled by the operation system. Due to handling of interrrupt takes some time then current process in the single processsor system will be paused. Thus any process in the system have unpredictable influence on perfomance of other processes regardless of their priorities.```
