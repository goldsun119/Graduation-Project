using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    public Camera Isometric_Cam;
    public Camera Third_Cam;
    public Camera First_Cam;

    // player 넣고 플레이어 받아와서 카메라주기?
    
    public float mouseSensitivity = 3f;
    public float upDownRange = 90;

    public int Camera_Num;

    void Start()
    {
        ThirdCamOn();
        Camera_Num = 0;

    }

    // Update is called once per frame
    void Update()
    {
        if(Input.GetKeyUp(KeyCode.V))
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
    }

    void FirstCamOn()
    {
        Third_Cam.enabled = false;
        First_Cam.enabled = true;
        Isometric_Cam.enabled = false;

        FirstCamMouseMove();
    }

    void IsometricCamOn()
    {
        Third_Cam.enabled = false;
        First_Cam.enabled = false;
        Isometric_Cam.enabled = true;
    }

    void IsometricCamMove()
    {
        transform.position = transform.parent.position;
    }

    void FirstCamMouseMove()
    {
        //좌우 회전
        float hor = Input.GetAxis("Mouse X");
        Debug.Log("move mouse");
        transform.parent.Rotate(Vector3.up * mouseSensitivity * hor);

        //상하 회전

        //float ver -= Input.GetAxis("Mouse Y") * mouseSensitivity;
        //ver = Mathf.Clamp(ver, -upDownRange, upDownRange);
        //Camera.main.transform.localRotation = Quaternion.Euler(ver, 0f, 0f);
    }

}
