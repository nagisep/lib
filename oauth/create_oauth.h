
#if !defined(_BCL_OAUTH_CREATE)
#define      _BCL_OAUTH_CREATE

#include <system.hpp>
#include <Classes.hpp>


#include "urlenc.h"
#include "hmac.h"

struct OAuthDataDef
{
   char *name;
   char *Consumer_key;
   char *Consumer_secret;
   char *Access_Token;
   char *Access_Token_Secret;
};

void makeNonce();
void makeTimeStamp();
void makeSignHere(AnsiString baseText, AnsiString key);
AnsiString makeBaseString(OAuthDataDef oauth, char *body = 0, bool isImage = false);
AnsiString makeHeadder(OAuthDataDef oauth);

#endif


