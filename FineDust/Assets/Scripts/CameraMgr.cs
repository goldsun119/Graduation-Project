using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraMgr : MonoBehaviour
{
    public Camera mainCam;     // 3인칭
    public Camera subCam;       // 1인칭

    public bool isCamMain = true;

    // Start is called before the first frame update
    void Start()
    {
        MainCamOn();
    }

    public void MainCamOn()
    {
        mainCam.enabled = true;
        subCam.enabled = false;
    }

    public void SubCamOn()
    {
        mainCam.enabled = false;
        subCam.enabled = true;
    }
}
