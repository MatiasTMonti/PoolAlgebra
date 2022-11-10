using UnityEngine;

public class CueStick : MonoBehaviour
{
    [SerializeField] private WhiteBall whiteBall;

    private float force;
    private float maxForce;
    private float minForce;

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.UpArrow) && force < maxForce)
        {
            AddForce();
        }
        if (Input.GetKeyDown(KeyCode.DownArrow) && force > minForce)
        {
            RemoveForce();
        }
        if (Input.GetKey(KeyCode.Space))
        {
            Shoot();
        }
    }


    private void AddForce()
    {
        force += 0.5f;
    }

    private void RemoveForce()
    {
        force -= 0.5f;
    }

    private void Shoot()
    {
        whiteBall.SetWhiteBallForce(force);
    }
}
