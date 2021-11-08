/******************************************************************************
* M5Core2
* (C)2021 M.Volkel
*
* Main-Program
*******************************************************************************/

// Comment templates

/******************************************************************************
*
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/

/******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>
#include <driver/i2s.h>

#include "main.h"
#include "gui/gui.h"
#include "frame/frame.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
char buffer[16];

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
- setup()
------------------------------------------------------------------------------*/
void setup()
{
	Serial.begin(115200);

	M5.begin(true, false, true, false);

	M5.Axp.SetLed(0);

	M5.Axp.SetSpkEnable(true);
	InitI2SSpeakerOrMic(MODE_SPK);
	xTaskCreatePinnedToCore(i2s_task, "i2s_task", 4096, NULL, 3, NULL, 0);

	M5.Axp.SetLed(1);

	M5.Lcd.clear(M5.Lcd.color565(149, 149, 149));
	M5.Lcd.drawBitmap(0, 0, 320, 24, titlebar);

	M5.Lcd.setFreeFont(&FreeSans9pt7b);
	M5.Lcd.setTextSize(1);
	M5.Lcd.setTextColor(BLACK);

	M5.Lcd.setCursor(8, 15);
	M5.Lcd.println("Amiga Workbench");

	Frame_Main *frame_main = new Frame_Main();
	GUI_PushFrame(frame_main);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void loop()
{
	GUI_MainLoop();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
bool InitI2SSpeakerOrMic(int mode)
{
	esp_err_t err = ESP_OK;
	i2s_driver_uninstall(Speak_I2S_NUMBER);
	i2s_config_t i2s_config = {
		.mode = (i2s_mode_t)(I2S_MODE_MASTER),
		.sample_rate = 44100,
		.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
		.channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
		.communication_format = I2S_COMM_FORMAT_I2S,
		.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
		.dma_buf_count = 2,
		.dma_buf_len = 128,
	};
	if (mode == MODE_MIC)
	{
		i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
	}
	else
	{
		i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
		i2s_config.use_apll = false;
		i2s_config.tx_desc_auto_clear = true;
	}
	err += i2s_driver_install(Speak_I2S_NUMBER, &i2s_config, 0, NULL);
	i2s_pin_config_t tx_pin_config;
	tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
	tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
	tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
	tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;
	err += i2s_set_pin(Speak_I2S_NUMBER, &tx_pin_config);
	err += i2s_set_clk(Speak_I2S_NUMBER, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
	return true;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void i2s_task(void *arg)
{
	size_t bytes_written = 0;
	i2s_write(Speak_I2S_NUMBER, previewR, 120264, &bytes_written, portMAX_DELAY);
	vTaskDelete(NULL);
}
