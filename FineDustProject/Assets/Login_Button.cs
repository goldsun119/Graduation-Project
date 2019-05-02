using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using System.Net;
using System.Net.Sockets;
using UnityEngine;


public class Login_Button : MonoBehaviour
{
    public InputField input_field;
    string server_ip_text;
    public IPAddress server_ip;
    Socket transferSock;

    // Start is called before the first frame update
    void Start()
    {
        input_field = GetComponent<InputField>();
    }

    // Update is called once per frame
    void Update()
    {
        Debug.Log(server_ip_text);
    }

    public void SceneChange()
    {
        SceneManager.LoadScene(1);
    }

    public void login()
    {
        server_ip_text = input_field.text;
        Debug.Log(server_ip_text);
        server_ip = IPAddress.Parse(server_ip_text);
        transferSock = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.IP);
        transferSock.Connect(new IPEndPoint(server_ip, 10801));

        SceneChange();
    }

}