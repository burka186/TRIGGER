//
//  Spring.cpp
//  firmataExample
//
//  Created by Ava Burke on 12/8/14.
//  Adapted from Charlie Whitney's Spring code, originally created on 11/11/13
//

#include "Spring.h"

void Spring::set(ofPoint _center, NoiseObjects *_s2, float _k, float _restLength) {
    center = _center;
    s2 = _s2;
    k = _k; //springiness constant
    restLength = _restLength;
}

void Spring::update() {
    //spring action
    ofPoint force = center - s2->pos;
    float len = force.length(); //same as calculating distance between the center and the point
    float stretchLength = len - restLength; //how far will it stretch?
    force.normalize(); //you want to do this or else SHIT WILL GET CRAZY
    force *= -1 * k * stretchLength;
    
    s2-> addForce( -force );
}

void Spring::draw() {
    //these lines will be invisible
    ofLine(center, s2->pos);
}