using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class Charactor_Select : MonoBehaviour
{
    public GameObject b1;
    public GameObject b2;
    public GameObject b3;
    public GameObject b4;

    public int s_num;
    public int num;


    public Image select_box;
    public Sprite ClearImg;
    public Sprite BoxImg;
    //public new Camera camera;
    //public Transform target;

    // Start is called before the first frame update
    void Start()
    {
        select_box = transform.GetComponent<Image>();
    }

    // Update is called once per frame
    void Update()
    {
        //if (Input.GetMouseButton(0))  // 마우스가 클릭 되면
        //{
        //    Vector3 mos = Input.mousePosition;
        //    mos.z = camera.farClipPlane; // 카메라가 보는 방향과, 시야를 가져온다.

        //    Vector3 dir = camera.ScreenToWorldPoint(mos);
        //    // 월드의 좌표를 클릭했을 때 화면에 자신이 보고있는 화면에 맞춰 좌표를 바꿔준다.

        //    RaycastHit hit;
        //    if (Physics.Raycast(transform.position, dir, out hit, mos.z))
        //    {
        //        target.position = hit.point; // 타겟을 레이캐스트가 충돌된 곳으로 옮긴다.
        //    }
        //}
    }

    public void Select1()
    {
        Image i = b1.transform.GetComponent<Image>();
        i.sprite = BoxImg;
        s_num = 1;
        b2.transform.GetComponent<Image>().sprite = ClearImg;
        //select_box = transform.GetComponent<Image>();
        //select_box.sprite = BoxImg;
        ////select_box.enabled = true;
        //Debug.Log("클릭은 됩니다.");
        //Debug.Log(this.name);
        //Debug.Log(num + "+" + s_num);
        //select_box.transform.transform.position = new Vector3(transform.position.x, 15, 0);
    }

    public void Select2()
    {
        Image i = b2.transform.GetComponent<Image>();
        i.sprite = BoxImg;
        s_num = 2;
        b1.transform.GetComponent<Image>().sprite = ClearImg;
        //select_box = transform.GetComponent<Image>();
        //select_box.sprite = BoxImg;
        ////select_box.enabled = true;
        //Debug.Log("클릭은 됩니다.");
        //Debug.Log(this.name);
        //Debug.Log(num + "+" + s_num);
        //select_box.transform.transform.position = new Vector3(transform.position.x, 15, 0);
    }

    public void SelectBoxOff()
    {
        select_box.sprite = ClearImg;
        //select_box.enabled = true;
        Debug.Log("클릭은 됩니다2.");
        //select_box.transform.transform.position = new Vector3(transform.position.x, 15, 0);
    }

    public void ChangeScene()
    {
        SceneManager.LoadScene(2);
    }

}
