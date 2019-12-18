#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED

enum Pitches {
  Null,
  B_0 = 31, C_1 = 33, CS_1 = 35, D_1 = 37, DS_1 = 39, E_1 = 41, F_1 = 44, FS_1 = 46, G_1 = 49, GS_1 = 52, A_1 = 55, AS_1 = 58,
  B_1 = 62, C_2 = 65, CS_2 = 69, D_2 = 73, DS_2 = 78, E_2 = 82, F_2 = 87, FS_2 = 93, G_2 = 98, GS_2 = 104, A_2 = 110, AS_2 = 117,
  B_2 = 123, C_3 = 131, CS_3 = 139, D_3 = 147, DS_3 = 156, E_3 = 165, F_3 = 175, FS_3 = 185, G_3 = 196, GS_3 = 208, A_3 = 220, AS_3 = 233,
  B_3 = 247, C_4 = 262, CS_4 = 277, D_4 = 294, DS_4 = 311, E_4 = 333, F_4 = 349, FS_4 = 370, G_4 = 392, GS_4 = 415, A_4 = 440, AS_4 = 466,
  B_4 = 494, C_5 = 523, CS_5 = 554, D_5 = 587, DS_5 = 622, E_5 = 659, F_5 = 698, FS_5 = 740, G_5 = 784, GS_5 = 831, A_5 = 880, AS_5 = 932,
  B_5 = 988, C_6 = 1047, CS_6 = 1109, D_6 = 1175, DS_6 = 1245, E_6 = 1319, F_6 = 1397, FS_6 = 1480, G_6 = 1568, GS_6 = 1661, A_6 = 1760, AS_6 = 1865,
  B_6 = 1976, C_7 = 2093, CS_7 = 2217, D_7 = 2349, DS_7 = 2489, E_7 = 2637, F_7 = 2794, FS_7 = 2960, G_7 = 3136, GS_7 = 3322, A_7 = 3520, AS_7 = 3729,
  B_7 = 3951, C_8 = 4186, CS_8 = 4435, D_8 = 4699, DS_8 = 4978,
} vTwinkle[] = {
  Pitches::C_4, Pitches::C_4, Pitches::G_4, Pitches::G_4, Pitches::A_4, Pitches::A_4, Pitches::G_4, Pitches::Null,
  Pitches::F_4, Pitches::F_4, Pitches::E_4, Pitches::E_4, Pitches::D_4, Pitches::D_4, Pitches::C_4, Pitches::Null,
  Pitches::G_4, Pitches::G_4, Pitches::F_4, Pitches::F_4, Pitches::E_4, Pitches::E_4, Pitches::D_4, Pitches::Null,
  Pitches::G_4, Pitches::G_4, Pitches::F_4, Pitches::F_4, Pitches::E_4, Pitches::E_4, Pitches::D_4, Pitches::Null,
  Pitches::C_4, Pitches::C_4, Pitches::G_4, Pitches::G_4, Pitches::A_4, Pitches::A_4, Pitches::G_4, Pitches::Null,
  Pitches::F_4, Pitches::F_4, Pitches::E_4, Pitches::E_4, Pitches::D_4, Pitches::D_4, Pitches::C_4, Pitches::Null
},vOdeToJoy[] = {
  Pitches::FS_4, Pitches::FS_4, Pitches::G_4, Pitches::A_4, Pitches::A_4, Pitches::G_4, Pitches::FS_4, Pitches::E_4,
  Pitches::D_4, Pitches::D_4, Pitches::E_4, Pitches::FS_4, Pitches::FS_4, Pitches::Null, Pitches::E_4, Pitches::E_4, Pitches::Null,
  Pitches::FS_4, Pitches::FS_4, Pitches::G_4, Pitches::A_4, Pitches::A_4, Pitches::G_4, Pitches::FS_4, Pitches::E_4,
  Pitches::D_4, Pitches::D_4, Pitches::E_4, Pitches::FS_4, Pitches::E_4, Pitches::Null, Pitches::D_4, Pitches::D_4, Pitches::Null,
  Pitches::E_4, Pitches::E_4, Pitches::FS_4, Pitches::D_4,
  Pitches::E_4, Pitches::FS_4, Pitches::G_4, Pitches::FS_4, Pitches::D_4,
  Pitches::E_4, Pitches::FS_4, Pitches::G_4, Pitches::FS_4, Pitches::E_4, Pitches::D_4, Pitches::E_4, Pitches::A_3, Pitches::Null,
  Pitches::FS_4, Pitches::Null, Pitches::FS_4, Pitches::G_4, Pitches::A_4, Pitches::A_4, Pitches::G_4, Pitches::FS_4, Pitches::E_4,
  Pitches::D_4, Pitches::D_4, Pitches::E_4, Pitches::FS_4, Pitches::E_4, Pitches::Null, Pitches::D_4, Pitches::D_4,
};

class Song {
    String Name_;
    Pitches*  Pitch_;
    int8_t Length_, Position_, SizeUnit_;
    int16_t TimeUnit_;
  public:
    Song(const String& Name = "Unknown", const Pitches* const  Pitch = nullptr, const int8_t& Length = 0, const int8_t& SizeUnit = 0, const int16_t& TimeUnit = 0):  Name_(Name), Pitch_(new Pitches[Length]), Length_(Length), Position_(0), TimeUnit_(TimeUnit), SizeUnit_(SizeUnit) {
      if ( Pitch == nullptr) {
        for (int i = 0; i < Length_; ++i)
          Pitch_[i] = Pitches::Null;
      } else {
        for (int i = 0; i < Length_; ++i)
          Pitch_[i] =  Pitch[i];
      }
    }
    Song& operator=(const Song& Obj) {
      if (this == &Obj)
        return *this;
      Name_ = Obj.Name_;
      Length_ = Obj.Length_;
      delete[]  Pitch_;
      Pitch_ = new Pitches[Length_];
      for (int i = 0; i < Length_; ++i)
        Pitch_[i] = Obj. Pitch_[i];
      Position_ = 0;
      TimeUnit_ = Obj.TimeUnit_;
      SizeUnit_ = Obj.SizeUnit_;
      return *this;
    }
    ~Song() {
      delete[] Pitch_;
    }
    const String& Name() const {
      return Name_;
    }
    const Pitches& operator[](const int8_t& Position) const {
      return  Pitch_[Position];
    }
    inline Pitches* const  Pitch() const {
      return  Pitch_;
    }
    inline const int8_t& Length() const {
      return Length_;
    }
    inline int8_t& Position() {
      return Position_;
    }
    inline const int8_t& SizeUnit() const {
      return SizeUnit_;
    }
    inline const int16_t& TimeUnit() const {
      return TimeUnit_;
    }
} Songs[] = {{"Twinkle", vTwinkle, 48, 3, 750}, {"OdeToJoy", vOdeToJoy, 69, 3, 375}};

int8_t songNo = sizeof(Songs)/sizeof(Song);

#endif
