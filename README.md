# HydroPress: Real-Time Hydrostatic Pressure Simulator

HydroPress adalah simulasi teknik interaktif yang dirancang untuk memodelkan variasi tekanan hidrostatik berdasarkan kedalaman secara real-time. Dikembangkan menggunakan **C++** dan **Qt Framework**, aplikasi ini menggunakan *discrete-time simulation loop* untuk mendekati perubahan tekanan kontinu selama proses penurunan.

### Key Features
* **Physics Model:** Menghitung tekanan hidrostatik menggunakan rumus $P = P_{atm} + \rho gh$.
* **Real-Time Simulation:** Memperbarui data kedalaman, tekanan, dan status sistem pada interval waktu tetap.
* **Status Evaluation:** Mengklasifikasikan kondisi struktur ke dalam status **SAFE**, **CRITICAL**, dan **FAILURE**.
* **Interactive Dashboard:** Menampilkan indikator kedalaman, tekanan, dan status secara langsung.
* **User Controls:** Menyediakan konfigurasi preset dan kustom terbatas dengan parameter yang dikunci selama simulasi berjalan.
