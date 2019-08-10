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
    public Camera MiniMap_Cam;
    public Camera NPC_Cam;

    public bool NPC_On;

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
        NPC_On = false;
        NPC_Cam.enabled = false;

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

        MinimapCamMove();
        if (!NPC_On)
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

    public void NPCCamOn(GameObject NPC)
    {
        NPC_On = true;

        Isometric_Cam.enabled = false;
        Third_Cam.enabled = false;
        First_Cam.enabled = false;
        NPC_Cam.enabled = true;

        Cursor.lockState = CursorLockMode.None;
        NPCCamMove(NPC);
    }

    public void NPCCamOff(GameObject NPC)
    {
        NPC_On = false;
        NPC_Cam.enabled = false;
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

    void ThirdCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        Third_Cam.transform.rotation = Quaternion.Euler(player.rotation.x + 15, player.rotation.y, player.rotation.z);
        Third_Cam.transform.position = rotation * new Vector3(0, 4.25f, -5.5f) + player.position;
    }

    void FirstCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        First_Cam.transform.rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        First_Cam.transform.position = new Vector3(player.position.x, player.position.y + 1.6f, player.position.z + 0.2f);
    }

    void MinimapCamMove()
    {
        Quaternion rotation = Quaternion.Euler(player.rotation.x, player.rotation.y, player.rotation.z);
        MiniMap_Cam.transform.rotation = Quaternion.Euler(90.0f, player.rotation.y, 0.0f);
        MiniMap_Cam.transform.position = new Vector3(player.position.x, 200.0f, player.position.z);
    }

    void IsometricCamMove()
    {
        Isometric_Cam.transform.position = new Vector3(player.position.x - 5, player.position.y + 10f, player.position.z - 5);
        //Debug.Log(transform.position.x);
        //Debug.Log(Isometric_Cam.transform.position.x);
    }

    void NPCCamMove(GameObject NPC)
    {
        //Quaternion rotation = Quaternion.Euler(NPC.transform.rotation.x, NPC.transform.rotation.y, NPC.transform.rotation.z);
        NPC_Cam.transform.rotation = Quaternion.Euler(NPC.transform.rotation.x + 15, NPC.transform.rotation.y + 180, NPC.transform.rotation.z);
        NPC_Cam.transform.position = new Vector3(NPC.transform.position.x, NPC.transform.position.y + 1.4f, NPC.transform.position.z + 2.0f);
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
