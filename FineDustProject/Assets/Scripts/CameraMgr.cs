using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    GameObject playerObj;
    PlayerStatus player;
    
    public Camera Third_Cam;
    public Camera First_Cam;
    public Camera Isometric_Cam;

    // player 넣고 플레이어 받아와서 카메라주기?

    public float mouseSensitivity = 3f;
    public float upDownRange = 90;

    public int Camera_Num;

    void Start()
    {
        string a = "Player(" + Game.Network.NetWork.Client_id.ToString() + ")";
        playerObj = GameObject.Find("Players").transform.Find(a).gameObject;
        player = playerObj.GetComponent<PlayerStatus>();
        
        ThirdCamOn();
        Camera_Num = 0;

    }

    // Update is called once per frame
    void Update()
    {
        player = playerObj.GetComponent<PlayerStatus>();

        if (Input.GetKeyUp(KeyCode.V))
        {
            Camera_Num += 1;
            Camera_Num = Camera_Num % 3;
            Debug.Log(Camera_Num);
        }

        switch (Camera_Num)
        {
            case 0:
                ThirdCamOn();
                break;
            case 1:
                FirstCamOn();
                break;
            case 2:
                IsometricCamOn();
                break;
            default:
                IsometricCamOn();
                break;
        }
    }


    void ThirdCamOn()
    {
        Third_Cam.enabled = true;
        First_Cam.enabled = false;
        Isometric_Cam.enabled = false;

        ThirdCamMove();
    }

    void FirstCamOn()
    {
        First_Cam.enabled = true;
        Third_Cam.enabled = false;
        Isometric_Cam.enabled = false;

        Cursor.lockState = CursorLockMode.Locked;
        
        FirstCamMouseMove();
        FirstCamMove();
    }

    void IsometricCamOn()
    {
        Isometric_Cam.enabled = true;
        Third_Cam.enabled = false;
        First_Cam.enabled = false;

        Cursor.lockState = CursorLockMode.None; //: 일반 마우스 커서
        //Cursor.lockState = CursorLockMode.Confined : 창밖으로 커서가 나가지 못함
        //Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y + 45, player.rotation.z);
        //player.transform.rotation = rotation;
        //player.transform.position = rotation * player.position;
        IsometricCamMove();
    }

    void ThirdCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        Third_Cam.transform.rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        Third_Cam.transform.position = rotation * new Vector3(0, 2.2f, - 4.5f) + player.position;
    }

    void FirstCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        First_Cam.transform.rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        First_Cam.transform.position = new Vector3(player.position.x, player.position.y + 1.6f, player.position.z + 0.2f);
    }


    void IsometricCamMove()
    {
        Isometric_Cam.transform.position = new Vector3(player.position.x - 10f, player.position.y + 10f, player.position.z - 10f);
        //Debug.Log(transform.position.x);
        //Debug.Log(Isometric_Cam.transform.position.x);
    }

    void FirstCamMouseMove()
    {
        //좌우 회전
        float hor = Input.GetAxis("Mouse X");
        Debug.Log("move mouse");
        player.transform.Rotate(Vector3.up * mouseSensitivity * hor);

        //상하 회전

        //float ver -= Input.GetAxis("Mouse Y") * mouseSensitivity;
        //ver = Mathf.Clamp(ver, -upDownRange, upDownRange);
        //Camera.main.transform.localRotation = Quaternion.Euler(ver, 0f, 0f);
    }

}
