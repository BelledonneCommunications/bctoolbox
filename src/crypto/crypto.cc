#include <bctoolbox/crypto.hh>
#include <iostream>

static std::vector<uint8_t> F(const std::vector<uint8_t>& password,std::vector<uint8_t> salt, int c, uint32_t i){
    std::vector<uint8_t> U;
    std::vector<uint8_t> Ures;
    for(int j = 0 ; j < c ; j++){
        if(j == 0){
            for(int k = 3 ; k >= 0 ; k--){
                salt.insert(salt.end(), (uint8_t)i>>(k*8));
            }
            U = bctoolbox::HMAC<bctoolbox::SHA256>(password, salt);
            Ures = U;
        } else {
            U = bctoolbox::HMAC<bctoolbox::SHA256>(password, U);
            for(size_t l = 0 ; l < Ures.size() ; l++){
                Ures.at(l) ^= U.at(l);
            }
        }
    }
    return Ures;
}


std::vector<uint8_t> PBKDF2_HMAC_SHA_256(const std::string password, const std::string salt, int c, size_t dkLen){
    const std::vector<uint8_t> P(password.begin(), password.end());
    const std::vector<uint8_t> S(salt.begin(), salt.end());
    std::vector<uint8_t> DK;
    std::vector<uint8_t> DKres;
    std::vector<uint8_t> T;
    std::vector<uint8_t>::const_iterator first;
    std::vector<uint8_t>::const_iterator last;
    size_t hLen = 32;
    uint32_t stop = (uint32_t)(dkLen/hLen);
    if(dkLen%hLen != 0) stop++;
    for(uint32_t i = 1 ; i <= stop ; i++){
        T = F(P, S, c, i);
        if(i == 1){
            DK = T;
        } else {
            DK.insert(DK.end(), T.begin(), T.end());
        }
    }
    first = DK.begin();
    last = DK.begin() + dkLen;
    DKres.assign(first, last);
    return DKres;
}
