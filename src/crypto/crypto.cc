#include <bctoolbox/crypto.hh>

std::vector<uint8_t> PBKDF2(const std::vector<uint8_t> password, const std::vector<uint8_t> salt, int c, size_t dkLen){
    std::vector<uint8_t> DK;
    std::vector<uint8_t> T;
    uint32_t hLen = 256;
    //Concaténation des Ti
    for(uint32_t i = 0 ; i < (dkLen/hLen) ; i++){
        T = F(password, salt, c, i);
        if(i == 0){
            DK = T;
        } else {
            for(size_t j = 0 ; j < T.size() ; j++){
                DK.insert(DK.end(), T.at(j));
            }
        }
    }
    return DK;
}

std::vector<uint8_t> F(const std::vector<uint8_t>& password,std::vector<uint8_t> salt, int c, uint32_t i){
    std::vector<uint8_t> U;
    std::vector<uint8_t> Ures;
    for(int j = 0 ; j < c ; j++){
        if(j == 0){
            const std::vector<std::uint8_t> vector_i((std::uint8_t*)&i, (std::uint8_t*)&(i) + sizeof(std::uint32_t));
            for(int k = 0 ; k < 4 ; k++){
                salt.insert(salt.end(), vector_i.at(k));
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
