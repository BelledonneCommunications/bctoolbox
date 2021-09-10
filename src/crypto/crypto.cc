#include <bctoolbox/crypto.hh>
#include <bctoolbox/crypto.h>

using namespace std;

namespace bctoolbox{

/**
 * @brief Xor iterations of chained HMAC-SHA-256
 *
 * @param[in] password  Word in which the HMAC-SHA-256 is applied
 * @param[in] salt      Cryptographic key
 * @param[in] c         Number of iterations
 * @param[in] i         Param to initialise first iteration
 *
 * @return  The xor iterations of chained HMAC-SHA-256 with param c and i
 */
static vector<uint8_t> F(const vector<uint8_t>& password, vector<uint8_t> salt, int c, uint32_t i){
    vector<uint8_t> U;
    vector<uint8_t> Ures;

    /* for j == 0 */
    //Concatenate salt with i in order of big endian
    for(int k = 3 ; k >= 0 ; k--){
        salt.insert(salt.end(), (uint8_t)i>>(k*8));
    }
    U = HMAC<bctoolbox::SHA256>(password, salt);
    Ures = U;

    for(int j = 1 ; j < c ; j++){
        U = HMAC<bctoolbox::SHA256>(password, U);
        transform(Ures.begin(), Ures.end(), U.begin(), Ures.begin(), bit_xor<uint8_t>{});
    }
    return Ures;
}


vector<uint8_t> PBKDF2_HMAC_SHA_256(const string& password, const string& salt, int c, size_t dkLen){
    vector<uint8_t> P(password.begin(), password.end());
    vector<uint8_t> S(salt.begin(), salt.end());
    vector<uint8_t> DK{};
    vector<uint8_t> T{};
    vector<uint8_t>::const_iterator first;
    vector<uint8_t>::const_iterator last;

    uint32_t i = 1;
    while(DK.size() < dkLen){
        T = F(P, S, c, i);
        DK.insert(DK.end(), T.begin(), T.end());
        i++;
    }

    bctbx_clean(P.data(), P.size());
    bctbx_clean(S.data(), S.size());

    //Resize DK to dkLen size
    DK.resize(dkLen);

    return DK;
}

} // namespace bctoolbox
