#include <bctoolbox/crypto.hh>

static std::vector<uint8_t> F(const std::vector<uint8_t>& password,std::vector<uint8_t> salt, int c, uint32_t i){
    std::vector<uint8_t> U;
    std::vector<uint8_t> Ures;
    for(int j = 0 ; j < c ; j++){
        if(j == 0){
            const std::vector<std::uint8_t> vector_i((std::uint8_t*)&i, (std::uint8_t*)&(i) + sizeof(std::uint32_t));
            salt.insert(salt.end(), vector_i.begin(), vector_i.end());
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
    std::vector<uint8_t> T;
    size_t hLen = 256;
    for(uint32_t i = 1 ; i <= (uint8_t)(dkLen/hLen) ; i++){
        T = F(P, S, c, i);
        if(i == 1){
            DK = T;
        } else {
            DK.insert(DK.end(), T.begin(), T.end());
        }
    }
    return DK;
}
