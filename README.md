# TDSOS
Лабораторні роботи з дисципліни "Теорія і технології проєктування спеціалізованих операційних систем"
## Лабораторна робота 1
 ### Завдання
>Складний рівень. Реалізувати етапи 1-4 проєктування спеціалізованої RTOS для платформи Raspberry Pi3. Протестувати поточну версію ядра та зробити відповідні висновки. Пояснити відмінності реалізації для платформи Raspberry Pi4. Оформити звіт з виконаної роботи.
>За результатами описаних етапів, можна зробити висновок, що реалізація спеціалізованої RTOS для Інтернету речей на платформі Raspberry Pi здійснюється шляхом поступової розробки кістяка проєкту, додавання підтримки мови Rust, розширення можливостей виведення повідомлень для налагодження, та реалізації псевдоблокування як примітиву синхронізації ОС. У цьому процесі ключову роль відіграють такі інструменти, як Makefile, QEMU та різноманітні засоби аналізу та перевірки коду, як objdump, nm, readelf. Описані кроки дозволяють розробникам поступово розширювати функціональність системи, забезпечуючи її безпечну та стабільну роботу на базі платформи Raspberry Pi.
 #### Відмінності реалізації.
> Найбільші різниці в реалізації етапів 1-4 між Raspberry Pi3 та Raspberry Pi4 пов'язані з їх апаратною конфігурацією та характеристиками. На Raspberry Pi4 можна очікувати значний приріст продуктивності та швидкодії за рахунок новітніх компонентів.
 #### Переваги Raspberry Pi 4 перед Raspberry Pi 3:
> -	Raspberry Pi 4 має більш потужний процесор (ARM Cortex-A72 vs ARM Cortex-A53 у Raspberry Pi 3) та більшу кількість оперативної пам'яті (до 8 ГБ проти 1 ГБ у Raspberry Pi 3);
> - Raspberry Pi 4 підтримує більше інтерфейсів (наприклад, USB 3.0, Gigabit Ethernet), що забезпечує більшу швидкість передачі даних і підвищує продуктивність;
> - Raspberry Pi 4 має підтримку технології Bluetooth 5.0, що дозволяє забезпечити більшу швидкість та дальність передачі даних;
> -	Raspberry Pi 4 має більшу швидкість зчитування та запису на microSD карту.
>> Отже, відмінності у реалізації етапів 1-4 на Raspberry Pi 3 та Raspberry Pi 4 в першу чергу в продуктивності. Завдяки більш потужному процесору та більшій кількості оперативної пам'яті, Raspberry Pi 4 може працювати швидше і ефективніше, що дозволить швидше виконувати завдання на кожному з етапів.
> Окрім цього, є різниця в технологічних особливостях між Raspberry Pi 3 та Raspberry Pi 4, таких як інтерфейси, розмір пам'яті, характеристики мікроконтролера тощо, що відіграє роль у виборі певних технічних рішень при реалізації кожного з етапів.
> У загальному, реалізація проєкту на обох платформах може відрізнятися в деталях, але основні етапи та процеси однакові.
 ## Лабораторна робота 2
 ### Завдання
> Реалізувати етапи 5-7 проєктування спеціалізованої RTOS для платформи Raspberry Pi3.
Протестувати поточну версію ядра та зробити відповідні висновки. Пояснити відмінності
реалізації для платформи Raspberry Pi4. Оформити звіт з виконаної роботи.
>Під час реалізації спеціалізованої RTOS для Інтернету речей на платформі Raspberry Pi 3 було зроблено значний прогрес. Було успішно реалізовано базові функції ядра, додані драйвери GPIO та UART, та реалізована функція завантаження ядра по UART. Крім того, були додані функції апаратного таймера, які допомогли уникнути циклічних затримок в драйвері GPIO та підвищили точність роботи. На завершення, був реалізований ланцюговий завантажувач, що дозволяє зручно завантажувати ядро через UART. Всі ці етапи були успішно пройдені, що дозволяє запускати код на реальному обладнанні та продовжувати розробку та тестування RTOS для Raspberry Pi 3.
> ### Різниця реалізації між Raspberry Pi 3 та Raspberry Pi 4.
>> Крім апаратної конфігурації на даних етапах є різниця у їх реалізації.
На п’ятому етапі за замовчуванням усі цілі Makefile створюються для RPi3. Щоб здійснити побудову для RPi 4, слід додати BSP=rpi4 до кожної цілі. Наприклад:
BSP=rpi4 make
BSP=rpi4 make doc 
Перші два кроки реалізації п’ятого етапу однакові, а саме:
> 1.	cтворити єдиний FAT32 розділ з іменем boot;
> 2.	згенерувати на карті файл config.txt з наступним вмістом:
> arm_64bit=1 
> init_uart_clock=48000000
>> ### Далі для Raspberry Pi 3 кроки 3-4 наступні:
>> 3.	слід скопіювати наступні файли з репозиторію прошивки Raspberry Pi https://github.com/raspberrypi/firmware/tree/master/boot на SD-карту:
>> -	bootcode.bin;
>> -	fixup.dat;
>> -	start.elf
>> 4.	виконати make.
> ### Далі для Raspberry Pi4 кроки 3-4 наступні:
> 3.	скопіювати наступні файли з репозиторію прошивки Raspberry Pi на SD-карту:
> -	fixup4.dat;
> -	start4.elf;
> -	bcm2711-rpi-4-b.dtb.
> 4.	виконайте BSP=rpi4 make.
> -  А уже наступні кроки для п’ятого етапу однакові для Raspberry Pi3 та Raspberry Pi4.
>> На шостому та сьомому етапі теж є розбіжності в залежності від версії обладнання на першому кроці слід запустити make (для RPi3) або BSP=rpi4 make (для RPi4).  А також на третьому кроці слід запустити make chainboot (для RPi3) або BSP=rpi4 make chainboot (для RPi4).
## Лабораторна робота 3
### Тема
Проєктування спеціалізованої RTOS для Інтернету речей на платформі Raspberry Pi. Реалізація рівнів привілеїв. Апаратне налагодження по JTAG. Реалізація рівнів привілеїв. Тестування на реальній платформі.
### Завдання
Складний рівень. Реалізувати етапи 8, 9 проєктування спеціалізованої RTOS для платформи Raspberry Pi3. Протестувати поточну версію ядра та зробити відповідні висновки. Пояснити відмінності реалізації для платформи Raspberry Pi4. Оформити звіт з виконаної роботи.
### Висновки
>Отже, під час виконання лабораторної роботи з налаштування апаратного забезпечення RPi з використанням JTAG (Joint Test Action Group) було реалізовано 8 та 9 етапи. Дані етапи дозволили налаштувати апаратне забезпечення RPi3 з використанням JTAG для відлагодження та програмування мікроконтролера. Завдяки цим крокам було здійснено налагодження програмного забезпечення на RPi, а саме зчитування та записування програмного коду в пам'ять мікроконтролера. Окрім того, за допомогою JTAG було отримано доступ до різних пінів GPIO для функціональності JTAG.
>Налагодження апаратного забезпечення слід було підключити ARM-USB-TINY-H до RPi (в даній роботі підключення відбувалося до RPi3). Відмінність підключення до RPi3 та RPi4 є так як розміщення портів відрізняється. 
>На Raspberry Pi 3 порти розташовані на одному боці пристрою:
>> -	4 порти USB 2.0 розташовані поруч зі собою.
>> -	Ethernet порт розташований поруч з USB портами.
>> -	HDMI порт розташований поруч з USB портами і Ethernet портом.
>> -	3.5 мм аудіороз'єм розташований поруч з HDMI портом.
>> -	MicroSD-слот знаходиться на зворотному боці плати.
> У Raspberry Pi 4 порти розташовані на двох боках пристрою:
>> -	2 порти USB 2.0 розташовані на одному боці поруч з HDMI портом.
>> -	2 порти USB 3.0 розташовані на протилежному боці поруч з Ethernet портом.
>> -	HDMI порт знаходиться на одному боці разом з USB 2.0 портами.
>> -	Ethernet порт розташований на протилежному боці поруч з USB 3.0 портами.
>> -	3.5 мм аудіороз'єм розташований на тому ж боці, що і HDMI порт.
>> -	MicroSD-слот знаходиться на зворотному боці плати, так само як і на RPi3.

> Отже, основні відмінності в підключенні та конфігурації JTAG-пристроїв між Raspberry Pi 4 і Raspberry Pi 3 включають наступне:
>> 1. Пін-розетка: Raspberry Pi 4 та Raspberry Pi 3 мають різні пін-розетки GPIO. Це означає, що піни JTAG можуть бути розташовані на різних місцях на роз'ємах GPIO для цих платформ. Відповідно, підключення JTAG-пристрою до відповідних пінів може відрізнятися.
>> 2. Налагоджувальна платформа: Для роботи з JTAG на Raspberry Pi 4 та Raspberry Pi 3 можуть використовуватися різні налагоджувальні платформи. Наприклад, для Raspberry Pi 4 можна використовувати OpenOCD (Open On-Chip Debugger), а для Raspberry Pi 3 - платформу GDB (GNU Debugger). Це може вимагати відмінних команд або параметрів конфігурації при запуску налагоджувального програмного забезпечення.
>> 3. Налагоджувальні інтерфейси: Raspberry Pi 4 має покращені можливості взаємодії з JTAG-пристроями, наприклад, включаючи підтримку високошвидкісного інтерфейсу USB 3.0. Це може забезпечити швидшу передачу даних між пристроями та платформою Raspberry Pi 4.


## Лабораторна робота 4
###Тема
