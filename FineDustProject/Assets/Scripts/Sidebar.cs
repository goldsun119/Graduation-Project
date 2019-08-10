using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Sidebar : MonoBehaviour
{
    public bool OnOff;
    public bool show;
     int target_show = 95 ; // -420
     int target_hide = -95; // -230

    public IEnumerator coroutine;

    // Start is called before the first frame update
    void Start()
    {
        OnOff = false;
        show = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown(KeyCode.Y))
            Debug.Log(transform.position.x);

        if (Input.GetKeyDown(KeyCode.U))
        {
            Debug.Log(transform.position.x);
            show = true;

            if (OnOff)
            {
                OnOff = false;
                //coroutine = HideSideBar();
                //StartCoroutine(coroutine);
            }
            else
            {
                OnOff = true;
                //coroutine = ShowSideBar();
                //StartCoroutine(coroutine);
            }
        }

        if (!OnOff && show)
        {
            HideSideBar();
            //coroutine = HideSideBar();
            //StartCoroutine(coroutine);
        }
        else if (OnOff && show)
        {
            ShowSideBar();
            //coroutine = ShowSideBar();
            //StartCoroutine(coroutine);
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
        //if (transform.position.x > target_hide)
        //{
        //    transform.Translate(Vector3.left);
        //}

        //else

        transform.position = new Vector3(-100, transform.position.y, transform.position.z);
        show = false;
        OnOff = false;
    }

    //IEnumerator ShowSideBar()
    //{
    //    while (true)
    //        if (transform.position.x < target_show)
    //        {
    //            transform.Translate(Vector3.right);
    //            yield return new WaitForSeconds(0.1f);
    //        }

    //}

    //IEnumerator HideSideBar()
    //{
    //    while (true)
    //        if (transform.position.x > target_hide)
    //        {
    //            transform.Translate(Vector3.left);
    //            yield return new WaitForSeconds(0.1f);
    //        }
    //}

}
