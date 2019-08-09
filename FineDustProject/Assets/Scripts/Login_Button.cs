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
    public InputField input_id;
    public InputField input_password;
    public static bool login_ok = false;
    //string server_ip_text;
    //public IPAddress server_ip;
    //Socket transferSock;

    // Start is called before the first frame update
    void Start()
    {
        input_field = GameObject.Find("InputAdd").GetComponent<InputField>();
        input_id = GameObject.Find("InputID").GetComponent<InputField>();
        input_password = GameObject.Find("InputPW").GetComponent<InputField>();

        login_ok = false;
    }

    // Update is called once per frame
    void Update()
    {
        //Debug.Log(server_ip_text);

        //if (login_ok == true)
        //    SceneManager.LoadScene(1);

        if (Game.Network.NetWork.SceneNum == 1)
            SceneManager.LoadScene(1);
        else if (Game.Network.NetWork.SceneNum == 2)
            SceneManager.LoadScene(2);
    }
    

    public void Login()
    {
        Debug.Log(input_id.text);
        Debug.Log(input_password.text);


        //server_ip_text = input_field.text;
        //server_ip = IPAddress.Parse(server_ip_text);
        Game.Network.NetWork.Instance_S.PlayerIP = input_field.text;
        Game.Network.NetWork.Instance_S.PlayerID = input_id.text;
        Game.Network.NetWork.Instance_S.PlayerPW = input_password.text;
        Game.Network.NetWork.Instance_S.connectSocket();
        //transferSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);
        //transferSock.Connect(new IPEndPoint(server_ip, 9000));
    }

    public void ChangeScene()
    {
        if (Game.Network.NetWork.SceneNum == 1)
            SceneManager.LoadScene(1);
        else if (Game.Network.NetWork.SceneNum == 2)
            SceneManager.LoadScene(2);
    }
}