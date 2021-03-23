.data
print_format:
	.string "count: %d\n"
array:
	.byte 12, 230, 67, 29, 129, 127, 102, 196, 200, 11
array_end:
	
.text
.global main

main:
	movl $0, %ecx /*sum of elements*/
	movl $array, %ebx /*addres of current element of array*/
	movzbl (%ebx), %eax /*definition of current element of array*/
	jmp ch_bound /*jump to compare or end of program*/

loop_start:
	or $107, %eax /*or with current element*/
	cmpl $128, %eax /*compare with 128*/
	jge less /*if current>=128 go to less*/

counterlow:
	addl $1, %ecx /*+1 to counter*/

less:
	addl $1, %ebx /*next element of array*/
	movzbl (%ebx), %eax /*write definition of next element to eax*/

ch_bound:
	cmpl $array_end, %ebx /*compare addresses of last and current element*/
	jne loop_start /*jump to loop if current element is not last*/
	pushl %ecx
	pushl $print_format
	call printf
	addl $8, %esp
	ret
