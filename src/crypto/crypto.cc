#include <bctoolbox/crypto.hh>

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
        for(size_t l = 0 ; l < Ures.size() ; l++){
            Ures.at(l) ^= U.at(l);
        }
    }
    return Ures;
}


vector<uint8_t> PBKDF2_HMAC_SHA_256(const string password, const string salt, int c, size_t dkLen){
    const vector<uint8_t> P(password.begin(), password.end());
    const vector<uint8_t> S(salt.begin(), salt.end());
    vector<uint8_t> DK;
    vector<uint8_t> DKres;
    vector<uint8_t> T;
    vector<uint8_t>::const_iterator first;
    vector<uint8_t>::const_iterator last;
    //Output size of the HMAC-SHA-256 function
    size_t hLen = 32;
    uint32_t stop = (uint32_t)(dkLen/hLen);

    //if hLen doesn't divide dkLen
    //Iterate once again
    if(dkLen%hLen != 0) stop++;

    /* for i == 1 */
    DK = F(P, S, c, 1);

    for(uint32_t i = 2 ; i <= stop ; i++){
        T = F(P, S, c, i);
        DK.insert(DK.end(), T.begin(), T.end());
    }

    //Resize DK to dkLen size
    first = DK.begin();
    last = DK.begin() + dkLen;
    DKres.assign(first, last);

    return DKres;
}

} // namespace bctoolbox
