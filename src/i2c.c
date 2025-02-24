#include "i2c.h"
/**
  * @brief  Initializes the I2Cx peripheral according to the specified 
  *         parameters in the I2C_InitStruct.
  * @param  I2Cx: the I2C peripheral.
  * @param  I2C_InitStruct: pointer to a I2C_InitTypeDef structure that 
  *			   contains the configuration information for the specified I2C peripheral.
  * 
  */
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct){

}

/**
  * @brief  Generates I2Cx communication START condition.
  * @param  I2Cx: the I2C peripheral.
  * @param  NewState: new state of the I2C START condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void I2C_GenerateStart(I2C_TypeDef * I2Cx, FunctionalState NewState){

}

/**
  * @brief  Generates I2Cx communication STOP condition.
  * @param  I2Cx: the I2C peripheral.
  * @param  NewState: new state of the I2C STOP condition generation.
  *          This parameter can be: ENABLE or DISABLE.
  * @retval None.
  */
void I2C_GenerateStop(I2C_TypeDef * I2Cx, FunctionalState NewState){

}
/**
  * @brief  Transmits the address byte to select the slave device.
  * @param  I2Cx: the I2C peripheral.
  * @param  Address: specifies the slave address which will be transmitted
  * @param  I2C_Direction: specifies whether the I2C device will be a Transmitter
  *         or a Receiver. 
  *          This parameter can be one of the following values
  *            @arg I2C_Direction_Transmitter: Transmitter mode
  *            @arg I2C_Direction_Receiver: Receiver mode
  * @retval None.
  */
void I2C_Send7bitAddress(I2C_TypeDef * I2Cx, uint8_t addrs, uint8_t I2C_Direction){

}

/**
  * @brief  Sends a data byte through the I2Cx peripheral.
  * @param  I2Cx: the I2C peripheral.
  * @param  Data: Byte to be transmitted..
  * @retval None
  */
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data){

}

/**
  * @brief  Returns the most recent received data by the I2Cx peripheral.
  * @param  I2Cx: the I2C peripheral.
  * @retval The value of the received data.
  */
uint16_t I2C_ReceiveData(I2C_TypeDef * I2Cx){

}
/**
  * @brief  Clears the I2Cx's pending flags.
  * @param  I2Cx: select the I2C peripheral.
  * @param  I2C_FLAG: specifies the flag to clear. 
  *          This parameter can be any combination of the following values:
  *            @arg I2C_FLAG_SMBALERT: SMBus Alert flag
  *            @arg I2C_FLAG_TIMEOUT: Timeout or Tlow error flag
  *            @arg I2C_FLAG_PECERR: PEC error in reception flag
  *            @arg I2C_FLAG_OVR: Overrun/Underrun flag (Slave mode)
  *            @arg I2C_FLAG_AF: Acknowledge failure flag
  *            @arg I2C_FLAG_ARLO: Arbitration lost flag (Master mode)
  *            @arg I2C_FLAG_BERR: Bus error flag
  *
  */
void I2C_ClearITFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG){
	
}

/**
  * @brief  Checks whether the specified I2C interrupt has occurred or not.
  * @param  I2Cx: the I2C peripheral.
  * @param  I2C_IT: specifies the interrupt source to check.
  */
bool I2C_GetItStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT){
	
	if (I2C_IT == 0x00) return true;
	return false;
}

