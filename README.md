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
>> Далі для Raspberry Pi 3 кроки 3-4 наступні:
>> 3.	слід скопіювати наступні файли з репозиторію прошивки Raspberry Pi https://github.com/raspberrypi/firmware/tree/master/boot на SD-карту:
>> -	bootcode.bin;
>> -	fixup.dat;
>> -	start.elf.
>? 4.	виконати make.
> Далі для Raspberry Pi4 кроки 3-4 наступні:
> 3.	скопіювати наступні файли з репозиторію прошивки Raspberry Pi на SD-карту:
> -	fixup4.dat;
> -	start4.elf;
> -	bcm2711-rpi-4-b.dtb.
> 4.	виконайте BSP=rpi4 make.
> А уже наступні кроки для п’ятого етапу однакові для Raspberry Pi3 та Raspberry Pi4.
>> На шостому та сьомому етапі теж є розбіжності в залежності від версії обладнання на першому кроці слід запустити make (для RPi3) або BSP=rpi4 make (для RPi4).  А також на третьому кроці слід запустити make chainboot (для RPi3) або BSP=rpi4 make chainboot (для RPi4).
