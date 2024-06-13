# message_queue_sample

Simple queue messageing sample for spresense using [Message Library](https://developer.sony.com/spresense/development-guides/sdk_developer_guide_ja.html#_message_library_2). 

"Message Library" is an inter-task synchronization library that allows sending and receiving class instances between tasks.

## HOW TO USE
Specify the main core number to be used and execute.
````
taskset 1 receiver & 
taskset 2 sender
````

**output example**
````
send_msg: 0 Hello, World!
receive_msg: 0 Hello, World!
send_msg: 1 Hello, World!
receive_msg: 1 Hello, World!
send_msg: 2 Hello, World!
receive_msg: 2 Hello, World!
send_msg: 3 Hello, World!
receive_msg: 3 Hello, World!
````