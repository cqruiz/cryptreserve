# [CryptReserve](https://www.blockchainbpi.com)
CryptReserve is a server written in C exposing IPFS and using Blockchain for permissioning and exposed via a RestAPI and communicating with IPFS and Geth using curl rpc.  


  ERC 1155  
    User Profile Contract
      User creates a Profile Contract that links to the Passport IPFS CID file.
      This ERC 1155 distributes burnable tokens to decrypt and view IPFS Attributes
      - CID
      - WalletId's - Permissions
      - CIDList (IPFS Attributes) (Coins/Personal Information Files/Permissioned/Encrypted)
        - CID is 21?
          - Wallet Ids (Permissioned to view)
        - CID is Verified Investor (>250k/yr)
          - Wallet Ids (Permissioned to view)

  IPFS    
    User Passport
      - Passport CID
      - WalletId
    Attributes
      - CID
      - Over 21 CID (Driver's License DMV)
      - Verify Mobile Number + MACID for GPS Verification
      - Verified Investor CID ()


## CryptReserve Installation Script
Run install.sh (./install.sh)

### Dependenices
* Linux or Mac OS
* IPFS
* C - gcc
* Jansson - library for encoding, decoding and manipulating JSON data
* Ulfius - libulfius-dev - HTTP Framework for REST Applications in C
* Orcania - LibOrcania-dev - Potluck of different fctns shared among C programs
* Hoel - Database (SQLIte3) abstraction library written in C.
* Yder - Logging library for C applications
* Glewlwyd - OAuth2
* OpenSSL
* JWT
* Libjwt
* libgnutls28-dev
