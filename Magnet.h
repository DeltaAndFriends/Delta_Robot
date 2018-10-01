#ifndef Magnet_h
#define Magnet_h

namespace Delta
{
  //! A magnet class
  /*!
   */
  class Magnet
  {
    public:
      Magnet();
      void setup();
      void switch_m(bool b);
    private:
      bool m_on{false};
  };
  
} // namespace Delta

#endif // Magnet_h
