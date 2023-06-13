#include &lt;linux/init.h&gt;
#include &lt;linux/module.h&gt;
#include &lt;linux/fs.h&gt;
#include &lt;linux/uaccess.h&gt;
#include &lt;linux/string.h&gt;
#define DEVICE_NAME &quot;chardev&quot;
#define BUFFER_SIZE 1024
static int major_number;
static char buffer[BUFFER_SIZE];
static int buffer_length;
static bool message_exists = false;

static int device_open(struct inode *inode, struct file *file)
{
printk(KERN_INFO &quot;Device opened\n&quot;);
return 0;
}
static int device_release(struct inode *inode, struct file *file)
{
printk(KERN_INFO &quot;Device released\n&quot;);
return 0;
}
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset)
{
int bytes_read = 0;
int i;
if (*offset &gt;= buffer_length)
return 0;
if (*offset + length &gt; buffer_length)
length = buffer_length - *offset;
for (i = 0; i &lt; length; i++) {
if (copy_to_user(&amp;user_buffer[i], &amp;buffer[*offset + i], 1))
return -EFAULT;
}
*offset += length;
return length;
}
static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t length, loff_t *offset)
{
int i;
if (length &gt;= BUFFER_SIZE)
return -EINVAL;
if (copy_from_user(buffer, user_buffer, length))
return -EFAULT;

buffer_length = length;
// Аналіз команди &quot;direction&quot;
if (strncmp(buffer, &quot;direction&quot;, 9) == 0) {
char direction[8];
sscanf(buffer, &quot;direction %[^\n]&quot;, direction);
printk(KERN_INFO &quot;Received direction command: %s\n&quot;, direction);
}
// Аналіз команди &quot;msg_delete&quot;
if (strncmp(buffer, &quot;msg_delete&quot;, 10) == 0) {
if (message_exists) {
memset(buffer, 0, BUFFER_SIZE);
buffer_length = 0;
message_exists = false;
printk(KERN_INFO &quot;Message deleted\n&quot;);
}
}
if (strncmp(buffer, &quot;message&quot;, 7) == 0) {
if (!message_exists) {
char message[BUFFER_SIZE];
sscanf(buffer, &quot;message %[^\n]&quot;, message);
strncpy(buffer, message, BUFFER_SIZE - 1);
buffer[BUFFER_SIZE - 1] = &#39;\0&#39;;
buffer_length = strlen(buffer);
message_exists = true;
printk(KERN_INFO &quot;Received message: %s\n&quot;, buffer);
}
}
printk(KERN_INFO &quot;Received data from user: &quot;);
for (i = 0; i &lt; length; i++)
printk(KERN_CONT &quot;%c&quot;, buffer[i]);
printk(KERN_CONT &quot;\n&quot;);
return length;
}
static struct file_operations fops = {
.open = device_open,
.release = device_release,
.read = device_read,

.write = device_write,
};
static int __init chardev_init(void)
{
major_number = register_chrdev(0, DEVICE_NAME, &amp;fops);
if (major_number &lt; 0) {
printk(KERN_ALERT &quot;Failed to register a major number\n&quot;);
return major_number;
}
printk(KERN_INFO &quot;Registered a major number %d\n&quot;, major_number);
return 0;
}
static void __exit chardev_exit(void)
{
unregister_chrdev(major_number, DEVICE_NAME);
printk(KERN_INFO &quot;Unregistered the major number %d\n&quot;, major_number);
}
module_init(chardev_init);
module_exit(chardev_exit);
MODULE_LICENSE(&quot;GPL&quot;);
MODULE_AUTHOR(&quot;Your Name&quot;);
MODULE_DESCRIPTION(&quot;A simple character device driver&quot;);