/**
 * \brief   С��ģʽ����
 * \note    ����С��ģʽ�жϺ��л�
 */
typedef enum {
    CONTROL_MODE_JOYSTICK,           /*!< ҡ�˿���ģʽ */
    CONTROL_MODE_EVADIBLE,           /*!< ����ģʽ */
    CONTROL_MODE_FOLLOW,             /*!< ����ģʽ */
    CONTROL_MODE_RGB_MODE_OFF,       /*!< RGB�ر� */
    CONTROL_MODE_RGB_MODE_BREATHING, /*!< RGB������ */
    CONTROL_MODE_RGB_MODE_RUNNING,   /*!< RGB��ˮ�� */
    CONTROL_MODE_RGB_MODE_FLASHING,  /*!< RGB��˸ */
} control_mode_t;

/**
 * \brief   RGB�ʵ���ʾģʽ
 * \note    ����RGB�ʵ���ʾģʽ�жϺ��л�
 */
typedef enum {
    RGB_MODE_OFF,       /*!< �ʵƹر� */
    RGB_MODE_BREATHING, /*!< ������ģʽ */
    RGB_MODE_RUNNING,   /*!< ��ˮ��ģʽ */
    RGB_MODE_FLASHING,  /*!< ��˸ģʽ */
} rgb_mode_t;
