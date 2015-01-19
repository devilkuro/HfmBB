/*
 * Key.h
 *
 *  Created on: 2015-1-4
 *      Author: bobo
 */

#ifndef KEY_H_
#define KEY_H_

namespace std {

class Key {
private:
	double key1;
	double key2;
public:
	Key();
	double getKey1();
	void setKey1(double key1);
	double getKey2();
	void setKey2(double key2);
	virtual ~Key();
};

} /* namespace std */
#endif /* KEY_H_ */
