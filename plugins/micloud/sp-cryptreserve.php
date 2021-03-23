<?php
/*
Plugin Name: Encrypted Document P2P Storage Plugin
Plugin URI: http://BlockchainBPI.com
Description: C IPFS Document Storage WordPress Form
        Interacts with the CryptReserve Node REST API
Version: 2.0.3
Author: Ed Vergara
Author URI: http://blockchainbpi.com
*/
    //
    // the plugin code will go here..

function html_createuser_form_code() {
    echo '<form action="' . esc_url( $_SERVER['REQUEST_URI'] ) . '" method="POST">';
    //echo '<form action="/cgi-bin/cr_createuser.cgi" method="post">';
    //echo '<form action="http://192.168.1.2:2884/auth/createuseraccount" method="PUT">';
    echo '<p>';
    echo 'User Name (required) <br />';
    echo '<input type="text" name="Name" pattern="[a-zA-Z0-9 ]+" value="' . ( isset( $_POST["Name"] ) ? esc_attr( $_POST["Name"] ) : '' ) . '" size="40" />';
    echo '</p>';
    echo '<p>';
    echo 'Your Email (required) <br />';
    echo '<input type="email" name="Email" value="' . ( isset( $_POST["Email"] ) ? esc_attr( $_POST["Email"] ) : '' ) . '" size="40" />';
    echo '</p>';
    echo '<p>';
    echo 'Password <br />';
    echo '<input type="password" name="Password" size="40" />';
    echo '</p>';
    echo '<p>';
    echo '<p><input type="submit" name="cf-submitted" value="Send"/></p>';
    echo '</form>';
}


function html_login_form_code() {
//    echo '<form action="' . esc_url( $_SERVER['REQUEST_URI'] ) . '" method="post">';
    echo '<form action="/cgi-bin/cr_login.cgi" method="post">';
    echo '<p>';
    echo 'User Name<br />';
    echo '<input type="text" name="cf-username" pattern="[a-zA-Z0-9 ]+" value="' . ( isset( $_POST["cf-username"] ) ? esc_attr( $_POST["cf-username"] ) : '' ) . '" size="40" />';
    echo '</p>';
    echo '<p>';
    echo 'Password <br />';
    echo '<input type="password" name="cf-password" size="40" />';
    echo '</p>';
    echo '<p><input type="submit" name="cf-submitted" value="Send"/></p>';
    echo '</form>';
}


function html_fileupload_form_code() {
    //echo '<form action="' . esc_url( $_SERVER['REQUEST_URI'] ) . '" method="post">';
    echo '<form action="/cgi-bin/cr_fileupload.cgi">';
    echo '<label for="myfile">Select files:</label>';
    echo '<input type="file" id="myfile" name="myfile"><br><br>';
//    echo '<input type="submit"/>';
    echo '<p><input type="submit" name="cf-submitted" value="Send"/></p>';
    echo '</form>';
}

function createuser() {
    // if the submit button is clicked, send the email
    if ( isset( $_POST['cf-submitted'] ) ) {
        $url = 'http://192.168.1.2:2884/auth/createuseraccount';
        // sanitize form values
        $name    = sanitize_text_field( $_POST["Name"] );
        $email   = sanitize_email( $_POST["Email"] );
        $password = sanitize_text_field( $_POST["Password"] );
        
        $bdy = array( 'Name' => $name,'Email' => $email, 'Password' => $password, 'Name' => $name);
        $jsonbdy = json_encode( $bdy );

        $contentlen = strlen($jsonbdy);
        $basicauth = 'Basic ' . base64_encode( $client_id . ':' . $client_secret );

        $headers = array( 
            'Authorization' => $basicauth,
            'Content-type' => 'application/json',
            'Content-length' => $contentlen
            );

        $pload = array(
            'method' => 'POST',
            'timeout' => 30,
            'redirection' => 5,
            'httpversion' => '1.0',
            'blocking' => true,
            'headers' => $headers,
            'body' => $jsonbdy,
            'cookies' => array()
            );

        $response = wp_remote_post($url, $pload);
    }
}

function cr_createuser() {
    ob_start();
    createuser();
    html_createuser_form_code();


    return ob_get_clean();
}

function cr_login() {
    ob_start();
    deliver_mail();
    html_login_form_code();

    return ob_get_clean();
}

function cr_fileupload() {
    ob_start();
    html_fileupload_form_code();

    return ob_get_clean();
} 

function ipfs_form_init() {
    
   // ob_start();
  // cr_createuser();
}

function enqueue_javascript() {
    //wp_enqueue_script('jsonparser', plugin_dir_url(__FILE__) . 'jsonparser.js');
    
}

add_action( 'init', 'ipfs_form_init', 0 );
add_action('javascript', 'enqueue_javascript');

add_shortcode( 'CR_login_form', 'cr_login' );
add_shortcode( 'CR_createuser_form', 'cr_createuser' );
add_shortcode( 'CR_fileupload_form', 'cr_fileupload' );

?>
