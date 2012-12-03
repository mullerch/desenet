#ifndef __libfixmath_fix16_hpp__
#define __libfixmath_fix16_hpp__

class Fix16 {
	public:
		fix16_t value;

		Fix16() 					 { value = 0; 						  }
		Fix16(const Fix16 &inValue)  { value = inValue.value;             }
		Fix16(const float inValue)   { value = fix16_from_float(inValue); }
		Fix16(const double inValue)  { value = fix16_from_dbl(inValue);   }
		Fix16(const int16_t inValue) { value = fix16_from_int(inValue);   }
		Fix16(const int inValue)     { value = fix16_from_int(inValue);   }

		static Fix16 fromFix16_t(const fix16_t inValue) { Fix16 f; f.value = inValue; return f; }

		operator double()  { return fix16_to_dbl(value);   }
		operator float()   { return fix16_to_float(value); }
		operator int16_t() { return fix16_to_int(value);   }
		operator int() 	   { return fix16_to_int(value);   }

		Fix16 & operator=(const Fix16 &rhs)  { value = rhs.value;             return *this; }
		Fix16 & operator=(const double rhs)  { value = fix16_from_dbl(rhs);   return *this; }
		Fix16 & operator=(const float rhs)   { value = fix16_from_float(rhs); return *this; }
		Fix16 & operator=(const int16_t rhs) { value = fix16_from_int(rhs);   return *this; }
		Fix16 & operator=(const int rhs)     { value = fix16_from_int(rhs);   return *this; }

		Fix16 & operator+=(const Fix16 &rhs)  { value += rhs.value;             return *this; }
		Fix16 & operator+=(const double rhs)  { value += fix16_from_dbl(rhs);   return *this; }
		Fix16 & operator+=(const float rhs)   { value += fix16_from_float(rhs); return *this; }
		Fix16 & operator+=(const int16_t rhs) { value += fix16_from_int(rhs);   return *this; }
		Fix16 & operator+=(const int rhs)     { value += fix16_from_int(rhs);   return *this; }

		Fix16 & operator-=(const Fix16 &rhs)  { value -= rhs.value; return *this; 				}
		Fix16 & operator-=(const double rhs)  { value -= fix16_from_dbl(rhs); return *this; 	}
		Fix16 & operator-=(const float rhs)   { value -= fix16_from_float(rhs); return *this; 	}
		Fix16 & operator-=(const int16_t rhs) { value -= fix16_from_int(rhs); return *this; 	}
		Fix16 & operator-=(const int rhs) 	  { value -= fix16_from_int(rhs); return *this; 	}

		Fix16 & operator*=(const Fix16 &rhs)  { value = fix16_mul(value, rhs.value); return *this; 				}
		Fix16 & operator*=(const double rhs)  { value = fix16_mul(value, fix16_from_dbl(rhs)); return *this; 	}
		Fix16 & operator*=(const float rhs)   { value = fix16_mul(value, fix16_from_float(rhs)); return *this; 	}
		Fix16 & operator*=(const int16_t rhs) { value *= rhs; return *this; 									}
		Fix16 & operator*=(const int rhs)     { value *= rhs; return *this; 									}

		Fix16 & operator/=(const Fix16 &rhs)  { value = fix16_div(value, rhs.value); return *this; 				}
		Fix16 & operator/=(const double rhs)  { value = fix16_div(value, fix16_from_dbl(rhs)); return *this; 	}
		Fix16 & operator/=(const float rhs)   { value = fix16_div(value, fix16_from_float(rhs)); return *this; 	}
		Fix16 & operator/=(const int16_t rhs) { value /= rhs; return *this; 									}
		Fix16 & operator/=(const int rhs)     { value /= rhs; return *this; 									}

		const Fix16 operator+(const Fix16 &other) const  { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const double other) const  { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const float other) const   { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const int16_t other) const { Fix16 ret = *this; ret += other; return ret; }
		const Fix16 operator+(const int other) const     { Fix16 ret = *this; ret += other; return ret; }

		const Fix16 sadd(const Fix16 &other)  const { return fromFix16_t( fix16_sadd(value, other.value) ); 			}
		const Fix16 sadd(const double other)  const { return fromFix16_t( fix16_sadd(value, fix16_from_dbl(other)) ); 	}
		const Fix16 sadd(const float other)   const { return fromFix16_t( fix16_sadd(value, fix16_from_float(other)) ); }
		const Fix16 sadd(const int16_t other) const { return fromFix16_t( fix16_sadd(value, fix16_from_int(other)) ); 	}
		const Fix16 sadd(const int other) const     { return fromFix16_t( fix16_sadd(value, fix16_from_int(other)) ); 	}

		const Fix16 operator-(const Fix16 &other) const  { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const double other) const  { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const float other) const   { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const int16_t other) const { Fix16 ret = *this; ret -= other; return ret; }
		const Fix16 operator-(const int other) const     { Fix16 ret = *this; ret -= other; return ret; }

		const Fix16 ssub(const Fix16 &other)  const { return fromFix16_t( fix16_sadd(value, -other.value) ); 				}
		const Fix16 ssub(const double other)  const { return fromFix16_t( fix16_sadd(value, -fix16_from_dbl(other)) ); 		}
		const Fix16 ssub(const float other)   const { return fromFix16_t( fix16_sadd(value, -fix16_from_float(other)) ); 	}
		const Fix16 ssub(const int16_t other) const { return fromFix16_t( fix16_sadd(value, -fix16_from_int(other)) ); 		}
		const Fix16 ssub(const int other) const     { return fromFix16_t( fix16_sadd(value, -fix16_from_int(other)) ); 		}

		const Fix16 operator*(const Fix16 &other) const  { Fix16 ret = *this; ret *= other; return ret; 	}
		const Fix16 operator*(const double other) const  { Fix16 ret = *this; ret *= other; return ret; 	}
		const Fix16 operator*(const float other) const   { Fix16 ret = *this; ret *= other; return ret; 	}
		const Fix16 operator*(const int16_t other) const { Fix16 ret = *this; ret *= other; return ret; 	}
		const Fix16 operator*(const int other) const     { Fix16 ret = *this; ret *= other; return ret; 	}

		const Fix16 smul(const Fix16 &other)  const { return fromFix16_t( fix16_smul(value, other.value) ); 			}
		const Fix16 smul(const double other)  const { return fromFix16_t( fix16_smul(value, fix16_from_dbl(other)) ); 	}
		const Fix16 smul(const float other)   const { return fromFix16_t( fix16_smul(value, fix16_from_float(other)) );	}
		const Fix16 smul(const int16_t other) const { return fromFix16_t( fix16_smul(value, fix16_from_int(other)) );	}
		const Fix16 smul(const int other) const     { return fromFix16_t( fix16_smul(value, fix16_from_int(other)) );	}

		const Fix16 operator/(const Fix16 &other) const  { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const double other) const  { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const float other) const   { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const int16_t other) const { Fix16 ret = *this; ret /= other; return ret; }
		const Fix16 operator/(const int other) const     { Fix16 ret = *this; ret /= other; return ret; }

		const Fix16 sdiv(const Fix16 &other)  const { return fromFix16_t( fix16_sdiv(value, other.value) ); 			}
		const Fix16 sdiv(const double other)  const { return fromFix16_t( fix16_sdiv(value, fix16_from_dbl(other)) ); 	}
		const Fix16 sdiv(const float other)   const { return fromFix16_t( fix16_sdiv(value, fix16_from_float(other)) ); }
		const Fix16 sdiv(const int16_t other) const { return fromFix16_t( fix16_sdiv(value, fix16_from_int(other)) ); 	}
		const Fix16 sdiv(const int other) const     { return fromFix16_t( fix16_sdiv(value, fix16_from_int(other)) ); 	}

		const int operator==(const Fix16 &other)  const { return (value == other.value);             }
		const int operator==(const double other)  const { return (value == fix16_from_dbl(other));   }
		const int operator==(const float other)   const { return (value == fix16_from_float(other)); }
		const int operator==(const int16_t other) const { return (value == fix16_from_int(other));   }
		const int operator==(const int other) const     { return (value == fix16_from_int(other));   }

		const int operator!=(const Fix16 &other)  const { return (value != other.value);             }
		const int operator!=(const double other)  const { return (value != fix16_from_dbl(other));   }
		const int operator!=(const float other)   const { return (value != fix16_from_float(other)); }
		const int operator!=(const int16_t other) const { return (value != fix16_from_int(other));   }
		const int operator!=(const int other) const 	{ return (value != fix16_from_int(other));   }

		const int operator<=(const Fix16 &other)  const { return (value <= other.value);             }
		const int operator<=(const double other)  const { return (value <= fix16_from_dbl(other));   }
		const int operator<=(const float other)   const { return (value <= fix16_from_float(other)); }
		const int operator<=(const int16_t other) const { return (value <= fix16_from_int(other));   }
		const int operator<=(const int other) const 	{ return (value <= fix16_from_int(other));   }

		const int operator>=(const Fix16 &other)  const { return (value >= other.value);             }
		const int operator>=(const double other)  const { return (value >= fix16_from_dbl(other));   }
		const int operator>=(const float other)   const { return (value >= fix16_from_float(other)); }
		const int operator>=(const int16_t other) const { return (value >= fix16_from_int(other));   }
		const int operator>=(const int other) const 	{ return (value >= fix16_from_int(other));   }

		const int operator< (const Fix16 &other)  const { return (value <  other.value);             }
		const int operator< (const double other)  const { return (value <  fix16_from_dbl(other));   }
		const int operator< (const float other)   const { return (value <  fix16_from_float(other)); }
		const int operator< (const int16_t other) const { return (value <  fix16_from_int(other));   }
		const int operator< (const int other) const 	{ return (value <  fix16_from_int(other));   }

		const int operator> (const Fix16 &other)  const { return (value >  other.value);             }
		const int operator> (const double other)  const { return (value >  fix16_from_dbl(other));   }
		const int operator> (const float other)   const { return (value >  fix16_from_float(other)); }
		const int operator> (const int16_t other) const { return (value >  fix16_from_int(other));   }
		const int operator> (const int other) const		{ return (value >  fix16_from_int(other));   }

		Fix16  sin() { return fromFix16_t(fix16_sin(value));  }
		Fix16  cos() { return fromFix16_t(fix16_cos(value));  }
		/*Fix16  tan() { return fromFix16_t(fix16_tan(value));  }
		Fix16 asin() { return fromFix16_t(fix16_asin(value)); }
		Fix16 acos() { return fromFix16_t(fix16_acos(value)); }
		Fix16 atan() { return fromFix16_t(fix16_atan(value)); }
		Fix16 atan2(const Fix16 &inY) { return fromFix16_t(fix16_atan2(value, inY.value)); }
		Fix16 sqrt() { return fromFix16_t(fix16_sqrt(value)); }*/
};

#endif
