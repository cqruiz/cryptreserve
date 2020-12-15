pragma solidity >=0.7.0 <0.8.0;

import "@openzeppelin/contracts/token/ERC1155/ERC1155.sol";

contract UserProfile is ERC1155 {
    uint256 public constant PUBLIC = 0;
    uint256 public constant FAMILY = 1;
    uint256 public constant FRIENDS = 2;
    uint256 public constant PROFESSIONAL = 3;
    uint256 public constant SOCCER = 4;
    uint256 cid;
    
    struct Contact {
        uint256 cid;
        address wallet;
        uint256 encr_pk_in;
        uint256 encr_pk_out;
    }

    Contact[] public contacts;

    constructor() public ERC1155("IPFS://BlockchainBPI.com/api/userprofile/{cid}.json") {
        _mint(msg.sender, PUBLIC, 10**18, "");
        _mint(msg.sender, FAMILY, 10**27, "");
        _mint(msg.sender, FRIENDS, 1, "");
        _mint(msg.sender, PROFESSIONAL, 10**9, "");
        _mint(msg.sender, SOCCER, 10**9, "");
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
  function getContactCount() public constant returns(uint contactCount) {
    return contacts.length;
  }

  //View Add with Callback to ClientProfile to verfiy the ad was viewed.


  function set (uint256 data) public {
    cid = data;
  }
  function add(uint256 cid, address wallet, uint encr_pk_out) public view returns (uint256) {


      return encr_pk_out;
  }
  function get() public view returns (uint256) {
    return cid;
  }
}