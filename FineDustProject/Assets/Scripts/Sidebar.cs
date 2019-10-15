using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sidebar : MonoBehaviour
{
    private bool OnOff;
    private bool show;
    private int target_show = 155 ; // -420
    private int target_hide = -95; // -230

    // Start is called before the first frame update
    void Start()
    {
        OnOff = false;
        show = false;
    }

    // Update is called once per frame
    void Update()
    {
        //if (Input.GetKeyDown(KeyCode.Y))

        if (Input.GetKeyDown(KeyCode.U))
        {
            show = true;

            if (OnOff)
            {
                OnOff = false;
            }
            else
            {
                OnOff = true;
            }
        }

        if (!OnOff && show)
        {
            HideSideBar();
        }
        else if (OnOff && show)
        {
            ShowSideBar();
        }

    }

    public void ShowSideBar()
    {
        if (transform.position.x < target_show)
        {
            transform.Translate(Vector3.right * 5);
        }

        else
             show = false;
    }

    public void HideSideBar()
    {
        transform.position = new Vector3(-180, transform.position.y, transform.position.z);
        show = false;
        OnOff = false;
    }

}
