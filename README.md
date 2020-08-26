# GTI5KW

Perangkat lunak ini utamanya akan  mengatur operasi pada GTI 5 KW untuk merubah energi dari panel surya yang berupa listrik dengan arus searah menjadi listrik dengan arus bulak balik. Listrik yang dikeluarkan akan di sinkronisasikan dengan listrik jala jalan PLN sehingga energi listrik yang didapat dari panel surya dapat langsung disalurkan ke jala jalan PLN. Perangkat lunak ini juga akan memastikan kondisi operasi akan selalu berada pada batas aman komponen yang digunakan. Alat ini didesain agar dapat di gunakan oleh operator dengan hanya memanfaatkan 4 buah tombol yang sudah terintegrasi dengan hardware, untuk itu perangkat lunak juga akan mengakomodasi input dari 4 buah tombol dengan ouput feedback kepada operator melalui 1 buat LCD 2x16. Perangkat lunak juga akan mengakomodasi input output dari serial RS-232 untuk kebutuhan debugging dan input output dari serial RS-485 untuk komunikasi dengan HMI. Perangkat keras yang digunakan adalah Infineon [XMC4500](https://www.infineon.com/cms/en/product/microcontroller/32-bit-industrial-microcontroller-based-on-arm-cortex-m/32-bit-xmc4000-industrial-microcontroller-arm-cortex-m4/xmc4500/).

Firmware yang digunakan memastikan fitur pengaman pada inverter bekerja dengan baik sehingga operasi yang dilakukan aman. Pengamanan berupa fungsi deteksi overvoltage, overcurrent, grid synchronizing, dan fitur pendeteksi komponen di dalam GTI seperti bagian IGBT dan komponen-komponen lainnya. Pemrograman menggunakan USB to Serial TTL dan dihubungkan langsung dengan XMC4500. Debugging dilakukan dengan mengecek output rs232 dari controller dan memastikan nilai-nilai sensor dan kontrol berada pada angka nominalnya.

 ![alt text](https://github.com/apawitan1995/GTI5KW/blob/master/GTI5kw.JPG?raw=true)

 
