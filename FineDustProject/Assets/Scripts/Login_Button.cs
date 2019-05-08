using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Net;
using System.Net.Sockets;


public class Login_Button : MonoBehaviour
{
    public InputField input_field;
    public static bool login_ok = false;
    //string server_ip_text;
    //public IPAddress server_ip;
    //Socket transferSock;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log(server_ip_text);

        if (login_ok == true)
            SceneManager.LoadScene(1);
    }
    

    public void Login()
    {
        input_field = gameObject.GetComponent<InputField>();
        //server_ip_text = input_field.text;
        //server_ip = IPAddress.Parse(server_ip_text);
        Game.Network.NetWork.Instance_S.PlayerIP = input_field.text;
        Game.Network.NetWork.Instance_S.connectSocket();
        //transferSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);
        //transferSock.Connect(new IPEndPoint(server_ip, 9000));
    }


}