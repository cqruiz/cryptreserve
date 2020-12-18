pragma solidity ^0.5.9;

/*
  ERC 1155 - TokenReceiver
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
*/

import "@openzeppelin/contracts/token/ERC1155/ERC1155.sol";

contract ClientProfile is ERC1155TokenReceiver {
    uint256 public constant PUBLIC = 0;
    uint256 public constant FAMILY = 1;
    uint256 public constant FRIENDS = 2;
    uint256 public constant PROFESSIONAL = 3;
    uint256 public constant SOCCER = 4;
    uint256 cid;
    
    struct User {
        uint256 cid;
        address wallet;
        uint256 encr_pk_in;
        uint256 encr_pk_out;
    }

    Contact[] public contacts;

    constructor() public ERC1155("IPFS://BlockchainBPI.com/api/clientprofile/{cid}.json") {
        _mint(msg.sender, GOLD, 10**18, "");
        _mint(msg.sender, SILVER, 10**27, "");
        _mint(msg.sender, THORS_HAMMER, 1, "");
        _mint(msg.sender, SWORD, 10**9, "");
        _mint(msg.sender, SHIELD, 10**9, "");
    }
  
  function addContact(address wallet, uint256 cid, uint pk) public returns(uint rowNumber) {
    Contact memory newContact;
    newContact.cid = cid;
    newContact.wallet = wallet;
    newContact.encr_pk_in = pk;

    return contacts.push(newContact)-1;
  }

  function removeContact(uint256 cid) public returns (uint wallet) {

  }
  function getContactCount() public returns(uint contactCount) {
    return contacts.length;
  }


  function set (uint256 data) public {
    cid = data;
  }
  function add(uint256 cid, address wallet, uint encr_pk_out) public view returns (uint256) {


      return encr_pk_out;
  }
  function get() public view returns (uint256) {
    return cid;
  }

    /**
      @notice Handle the receipt of a single ERC1155 token type.
      @dev An ERC1155-compliant smart contract MUST call this function on the token recipient contract, at the end of a `safeTransferFrom` after the balance has been updated.        
      This function MUST return `bytes4(keccak256("onERC1155Received(address,address,uint256,uint256,bytes)"))` (i.e. 0xf23a6e61) if it accepts the transfer.
      This function MUST revert if it rejects the transfer.
      Return of any other value than the prescribed keccak256 generated value MUST result in the transaction being reverted by the caller.
      @param _operator  The address which initiated the transfer (i.e. msg.sender)
      @param _from      The address which previously owned the token
      @param _id        The ID of the token being transferred
      @param _value     The amount of tokens being transferred
      @param _data      Additional data with no specified format
      @return           `bytes4(keccak256("onERC1155Received(address,address,uint256,uint256,bytes)"))`
  */
  function onERC1155Received(address _operator, address _from, uint256 _id, uint256 _value, bytes calldata _data) external returns(bytes4){

  }

  /**
      @notice Handle the receipt of multiple ERC1155 token types.
      @dev An ERC1155-compliant smart contract MUST call this function on the token recipient contract, at the end of a `safeBatchTransferFrom` after the balances have been updated.        
      This function MUST return `bytes4(keccak256("onERC1155BatchReceived(address,address,uint256[],uint256[],bytes)"))` (i.e. 0xbc197c81) if it accepts the transfer(s).
      This function MUST revert if it rejects the transfer(s).
      Return of any other value than the prescribed keccak256 generated value MUST result in the transaction being reverted by the caller.
      @param _operator  The address which initiated the batch transfer (i.e. msg.sender)
      @param _from      The address which previously owned the token
      @param _ids       An array containing ids of each token being transferred (order and length must match _values array)
      @param _values    An array containing amounts of each token being transferred (order and length must match _ids array)
      @param _data      Additional data with no specified format
      @return           `bytes4(keccak256("onERC1155BatchReceived(address,address,uint256[],uint256[],bytes)"))`
  */
  function onERC1155BatchReceived(address _operator, address _from, uint256[] calldata _ids, uint256[] calldata _values, bytes calldata _data) external returns(bytes4) {

  }

}