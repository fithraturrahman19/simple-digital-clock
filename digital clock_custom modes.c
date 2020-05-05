//jam_digital.c
//program jam digital menggunakan ATMega328 board arduino uno
//23 Februari 2018
//Muhammad Fithratur Rahman 13215005
//Sayyid Irsyadul Ibad 13215005

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

        int N; // variabel pengatur clock
        int A,B,C,D;
        int portd_1_int, portd_2_int, portd_3_int, portd_4_int;
        int portb_1, portb_2, portb_3, portb_4, portd_1, portd_2, portd_3, portd_4;
        int jam_puluhan;
        int jam_satuan;
        int menit_puluhan;
        int menit_satuan;
        int detik_puluhan;
        int detik_satuan;

///////////////////////////////////////////////////////////////////////
///////////////////// START MAIN PROGRAM //////////////////////////////
///////////////////////////////////////////////////////////////////////

int main () { 

        int mode_main = 0;
        
        // setting mode timer dan nilai awalnya
        TCCR1B |= (1<<CS02|(1<<CS00)); //inisialisasi timer 0
        TIMSK1 |=(1<<TOIE0);
        TCNT1 = 0x48E5; //initial of timer 0
        // setting mode interupsi
        EIMSK = 0x01;
        EIFR = 0x01;
        EICRA = 0x02;
        // setting port yang akan digunakan sebagai i/o
        DDRB = 0xFF;
        DDRD = 0xFF;
        DDRC = 0b000011;
        PORTC = 0xFF ;
        // enable global interrupt
        sei ();
        N = 0;
      
        portd_1 = 0b00000001;
        portd_2 = 0b00000001;
        portd_3 = 0b00000001;
        portd_4 = 0b00000001;
        portb_1 = 0b00100011;
        portb_2 = 0b00010011;
        portb_3 = 0b00001011;
        portb_4 = 0b00000111;
        
       while (1) {
        _delay_ms (1);
        PORTD = portd_1;
        PORTB = portb_1;
        _delay_ms (1);
        PORTD = portd_2;
        PORTB = portb_2;
        _delay_ms (1);
        PORTD = portd_3;
        PORTB = portb_3;
        _delay_ms (1);
        PORTD = portd_4;
        PORTB = portb_4;
      
        mode_main = keypad(mode_main);
      
        if (mode_main == 1) {
          setting (mode_main);
        } 

        if (mode_main == 2) {
          setting (mode_main);
        } 
        
        
       } //nutup while (1)
       
       return 0;
    } 

///////////////////////////////////////////////////////////////////////
///////////////////// END MAIN PROGRAM ////////////////////////////////
///////////////////////////////////////////////////////////////////////


// program ISR untuk perhitungan jam
// setiap satu detik masuk ISR dan digit menit bertambah satu 

 ISR(TIMER1_OVF_vect) {
  N++;
  if (N==20) {
  portd_1_int = converter(portd_1);
  portd_2_int = converter(portd_2);
  portd_3_int = converter(portd_3);
  portd_4_int = converter(portd_4);
  menit_satuan = portd_1_int + 1;
  menit_puluhan = portd_2_int;
  jam_satuan = portd_3_int;
  jam_puluhan = portd_4_int;
   
  //--------------------------------------------------
  if (detik_satuan == 10) {
    if (detik_puluhan == 5) {
      detik_puluhan = 0;
      menit_satuan += 1;
    }
    else {
      detik_puluhan += 1;
    }
    detik_satuan = 0;
  }

  if (menit_satuan == 10) {
    if (menit_puluhan == 5) {
      menit_puluhan = 0;
      jam_satuan += 1;
    }
    else {
      menit_puluhan += 1;
    }
    menit_satuan = 0;
  }

  if (jam_satuan == 10) {
    jam_puluhan += 1;
    jam_satuan = 0;
  }

  if ((jam_satuan == 4) && (jam_puluhan == 2)) {
    jam_puluhan = 0;
    jam_satuan = 0;
  } 
  //--------------------------------------------------------
  portd_1 = DtoNB (menit_satuan);
  portd_2 = DtoNB (menit_puluhan);
  portd_3 = DtoNB (jam_satuan);
  portd_4 = DtoNB (jam_puluhan);
  N = 0;
  }
  TCNT1 = 0x48E5;
 }

// END PROGRAM ISR
///////////////////

// fungsi converter hexadecimal to decimal
int converter (int toconvert) {

  int converted;
  if (toconvert == 0b10011011) {
    converted = 1;
  }
  else if (toconvert == 0b00100010) {
    converted = 2;
  }
  else if (toconvert == 0b00000001) {
    converted = 0;
  }
  else if (toconvert == 0b00001010) {
    converted = 3;
  }
  else if (toconvert == 0b10011000) {
    converted = 4;
  }
  else if (toconvert == 0b01001000) {
    converted = 5;
  }
  else if (toconvert == 0b01000000) {
    converted = 6;
  }
  else if (toconvert == 0b00011011) {
    converted = 7;
  }
  else if (toconvert == 0b00000000) {
    converted = 8;
  }
  else if (toconvert == 0b00001000) {
    converted = 9;
  }
  else {
    converted = 10;
  }
  return (converted);
}

// fungsi converter decimal to hexadecimal
int DtoNB(int angka)
  {
    int x;
    if (angka==1)
      x= 0b10011011;
    else if (angka==2)
      x=0b00100010;
    else if (angka==3)
      x=0b00001010;
    else if (angka==4)
      x=0b10011000;
    else if (angka==5)
      x=0b01001000;
    else if (angka==6)
      x=0b01000000;
    else if (angka==7)
      x=0b00011011;
    else if (angka==8)
      x=0b00000000;
    else if (angka==9)
      x=0b00001000;
    else if (angka ==0)
      x=0b00000001;
      else 
      x=0b00000001;
    return x;
  }

// fungsi pembaca nilai keypad
// keypad yang mungkin terbaca * atau #
// * menambah mode 1
// # menambah mode 2
int keypad (int mode_keypad) {
  int mode;
  mode = mode_keypad;
  
      // membaca input *
      PORTC &=~ (1<<PC1);    
      if (not(PINC &= (1<<2)));
        if(not(PINC &= (1<<2))){
          _delay_ms (500);
          mode += 1;
            if (mode>1) {
              mode = 0;
            }
       }
      PORTC |= (1<<PC1);

       // membaca input #
       PORTC &=~ (1<<PC1);
       if (not(PINC &= (1<<4)));
          if (not(PINC &= (1<<4))) {
            _delay_ms (500);
            mode += 2;
              if (mode>2) {
                mode = 0;
              }
         }
       PORTC |= (1<<PC1);
   
  return(mode);
}

void setting (int mode_setting) {
  // Pada Prosedur ini mode masuk dengan bernilai 1 atau 2
  // Pada keluar prosedur mode akan bernilai 0
  // Ada dua while untuk masing-masing mode
  int input_operasi(int op_before);
 
      while (mode_setting == 1) {
    
         _delay_ms (10);
        PORTD = portd_1;
        PORTB = portb_1;
        _delay_ms (10);
        PORTD = portd_2;
        PORTB = portb_2;
        _delay_ms (10);
        PORTD = portd_3;
        PORTB = portb_3;
        _delay_ms (10);
        PORTD = portd_4;
        PORTB = portb_4;
      
        PORTB &= 0b11111110;
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          portd_4_int = converter(portd_4);
          D = portd_4_int + 1;
          if (D >2) {
          D=0;
        }
        portd_4 = DtoNB (D);
        _delay_ms (200);
        }
        PORTB |= 0b00000001;
    
        PORTB &= 0b11111101; 
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          portd_3_int = converter(portd_3);
          C = portd_3_int + 1;
          if (D==2) {
          if (C>3) {
          C=0;
        }
        }
        portd_3 = DtoNB (C);
        _delay_ms (200);
        }
        PORTB |= 0b00000010;
     
        PORTC &= 0b11111110; 
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          portd_2_int = converter(portd_2);
          B = portd_2_int + 1;
          if (B>5) {
          B =0;
        }
         portd_2 = DtoNB (B);
         _delay_ms (200);
        }
        PORTC |= 0b00000001; 
     
        PORTC &= 0b11111101;    
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          portd_1_int = converter(portd_1);
          A = portd_1_int + 1;
          portd_1 = DtoNB (A);
          _delay_ms (200);
        }
        PORTC |= 0b00000010;  
        
        mode_setting = keypad (mode_setting);
      } // END WHILE (MODE SETTING == 1)

    //pengenalan variabel untuk kalkulator
    int num_1, num_2, result;
    int num_1_1 =0;
    int num_1_2 =0;
    int num_2_1 =0;
    int num_2_2 =0;
    int take_time = 0;
    int operasi;
    
      while (mode_setting == 2) {

            
            PORTD = 0b10000001;
            PORTB = portb_1;
            _delay_ms (200);
            PORTD = 0b10000001;
            PORTB = portb_2;
            _delay_ms (200);
            PORTD = 0b10000001;
            PORTB = portb_3;
            _delay_ms (200);
            PORTD = 0b10000001;
            PORTB = portb_4;
            _delay_ms (200);

            while (take_time == 0){
            int buff;
            // pengambilan nilai variabel pertama
            // pengisian digit pertama
            buff = 0;
            while (buff==0){
                  num_1_1 = input_num (num_1_1);
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    buff = 1;
                    _delay_ms (200);
                  }
                  PORTC |= 0b00000010;
                  
                  PORTD = DtoNB (num_1_1);
                  PORTB = 0b00000111;
                  _delay_ms (5);
            }

            //pengisian digit kedua
            buff = 0;
            while (buff==0){
                  num_1_2 = input_num (num_1_2);
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    buff = 1;
                    _delay_ms (200);
                  }
                  PORTC |= 0b00000010;
                  PORTD = DtoNB (num_1_2);
                  PORTB = 0b00001011;
                  _delay_ms (5);
                  PORTD = DtoNB (num_1_1);
                  PORTB = 0b00000111;
                  _delay_ms (5);
            }

            num_1 = num_1_1*10 + num_1_2;
            // pengambilan operasi hitung
            operasi = C;
            buff = 0;
            while (buff==0){
                  operasi = input_operasi(operasi);
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    buff = 1;
                    _delay_ms (200);
                  }
                  PORTC |= 0b00000010;

                  //hingga pengambilan operasi selesai dilakukan, menampilkan semua angka
                  
                  PORTD = DtoNB (num_1_1);
                  PORTB = 0b00000111;
                  _delay_ms (10);
                  
                  PORTD = DtoNB (num_1_2);
                  PORTB = 0b00001011;
                  _delay_ms (10);

                  PORTD = DtoNB (num_2_1);
                  PORTB = 0b00010011;
                  _delay_ms (10);

                  PORTD = DtoNB (num_2_2);
                  PORTB = 0b00100011;
                  _delay_ms (10);
            }

            // pengambilan nilai variabel kedua
            //pengisian digit pertama
            buff = 0;
            while (buff==0){
                  num_2_1 = input_num (num_2_1);
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    buff = 1;
                    _delay_ms (200);
                  }
                  PORTC |= 0b00000010;
                  PORTD = DtoNB (num_2_1);
                  PORTB = 0b00010011;
                  _delay_ms (5);
            }

            //pengisian digit kedua
            buff = 0;
            while (buff==0){
                  num_2_2 = input_num (num_2_2);
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    buff = 1;
                    _delay_ms (200);
                  }
                  PORTD = DtoNB (num_2_1);
                  PORTB = 0b00010011;
                  _delay_ms (5);
                  PORTC |= 0b00000010;
                  PORTD = DtoNB (num_2_2);
                  PORTB = 0b00100011;
                  _delay_ms (5);
            }

            num_2 = num_2_1*10 + num_2_2;

            
            //operasi hitung
            if (operasi == 1) {
            result = num_1 + num_2; }
            else if (operasi == 2) {
            result = num_1 - num_2; }
            else if (operasi == 3) {
            result = num_1 * num_2; }

            //display hasil
            int wait = 0;
            while (wait < 300) {
            display_num (result);
            _delay_ms (10);
            wait += 1;
            }
            
            take_time += 1;
            }
            

            // pengecekan apakah ada perubahan mode
            mode_setting = keypad (mode_setting);

            //jika mau hitung-hitung lagi
                  PORTC &= 0b11111101;    
                  if (not(PINC &= (1<<5)));
                  else if(not(PINC &= (1<<5))){
                    take_time -= 1;
                    _delay_ms (200);
                  }
                  PORTC |= 0b00000010;
                  
      }
      
} 


void display_num (int number) {
  
        // deklarasi variabel prosedur display
        int digit_1,digit_2,digit_3,digit_4;

        // pengambilan nilai per digit
        digit_1 = number/1000;
        digit_2 = number/100 - (number/1000)*10;
        digit_3 = number/10 - (number/100)*10;
        digit_4 = number - (number/10)*10;
        
        // menampilkan pada seven segment
         _delay_ms (10);
        PORTD = DtoNB (digit_4);
        PORTB = portb_1;
        _delay_ms (10);
        PORTD = DtoNB (digit_3);
        PORTB = portb_2;
        _delay_ms (10);
        PORTD = DtoNB (digit_2);
        PORTB = portb_3;
        _delay_ms (10);
        PORTD = DtoNB (digit_1);
        PORTB = portb_4;
        
    }

int input_num (int num_before) {
        int num;
        int buff=0;
        
            
             PORTB &= 0b11111110;
            if(not(PINC &= (1<<2)));
            else if(not(PINC &= (1<<2))){
              num = 1;
            }
            else if(not(PINC &= (1<<3))){
              num = 2;
            }
            else if(not(PINC &= (1<<4))){
              num = 3;
            }
             PORTB |= 0b00000001;

            PORTB &= 0b11111101;
            if(not(PINC &= (1<<2)));
            else if(not(PINC &= (1<<2))){
              num = 4;
            }
            else if(not(PINC &= (1<<3))){
              num = 5;
            }
            else if(not(PINC &= (1<<4))){
              num = 6;
            }
            PORTB |= 0b00000010;

            PORTC &= 0b11111110;
            if(not(PINC &= (1<<2)));
            else if(not(PINC &= (1<<2))){
              num = 7;
            }
            else if(not(PINC &= (1<<3))){
              num = 8;
            }
            else if(not(PINC &= (1<<4))){
              num = 9;
            }
            PORTC |= 0b00000001;

            PORTC &= 0b11111101;
            if(not(PINC &= (1<<3)));
             else if(not(PINC &= (1<<3))){
              num = 0;
             }
            PORTC |= 0b00000010;
      
      return (num);
}

int input_operasi (int op_before) {
        int op; //variabel penyimpan operasi
        // bisa A, B, atau C
        op = op_before;
        
        PORTB &= 0b11111110;
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
            op = 1;
        _delay_ms (200);
        }
        PORTB |= 0b00000001;
    
        PORTB &= 0b11111101; 
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          op = 2;
        _delay_ms (200);
        }
        PORTB |= 0b00000010;
     
        PORTC &= 0b11111110; 
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          op = 3;
         _delay_ms (200);
        }
        PORTC |= 0b00000001; 
        
        return (op);
}

void buff_d () {
    int buff = 0;
        while (buff == 0) {
        PORTC &= 0b11111101;    
        if (not(PINC &= (1<<5)));
        else if(not(PINC &= (1<<5))){
          buff = 1;
          _delay_ms (400);
        }
        PORTC |= 0b00000010;
        }
}
