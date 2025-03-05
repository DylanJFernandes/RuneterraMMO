﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/apigateway/APIGateway_EXPORTS.h>
#include <aws/apigateway/APIGatewayRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/apigateway/model/GatewayResponseType.h>
#include <utility>

namespace Aws
{
namespace APIGateway
{
namespace Model
{

  /**
   * <p>Clears any customization of a GatewayResponse of a specified response type on
   * the given RestApi and resets it with the default settings.</p><p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/apigateway-2015-07-09/DeleteGatewayResponseRequest">AWS
   * API Reference</a></p>
   */
  class DeleteGatewayResponseRequest : public APIGatewayRequest
  {
  public:
    AWS_APIGATEWAY_API DeleteGatewayResponseRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "DeleteGatewayResponse"; }

    AWS_APIGATEWAY_API Aws::String SerializePayload() const override;


    ///@{
    /**
     * <p>The string identifier of the associated RestApi.</p>
     */
    inline const Aws::String& GetRestApiId() const{ return m_restApiId; }
    inline bool RestApiIdHasBeenSet() const { return m_restApiIdHasBeenSet; }
    inline void SetRestApiId(const Aws::String& value) { m_restApiIdHasBeenSet = true; m_restApiId = value; }
    inline void SetRestApiId(Aws::String&& value) { m_restApiIdHasBeenSet = true; m_restApiId = std::move(value); }
    inline void SetRestApiId(const char* value) { m_restApiIdHasBeenSet = true; m_restApiId.assign(value); }
    inline DeleteGatewayResponseRequest& WithRestApiId(const Aws::String& value) { SetRestApiId(value); return *this;}
    inline DeleteGatewayResponseRequest& WithRestApiId(Aws::String&& value) { SetRestApiId(std::move(value)); return *this;}
    inline DeleteGatewayResponseRequest& WithRestApiId(const char* value) { SetRestApiId(value); return *this;}
    ///@}

    ///@{
    /**
     * <p>The response type of the associated GatewayResponse.</p>
     */
    inline const GatewayResponseType& GetResponseType() const{ return m_responseType; }
    inline bool ResponseTypeHasBeenSet() const { return m_responseTypeHasBeenSet; }
    inline void SetResponseType(const GatewayResponseType& value) { m_responseTypeHasBeenSet = true; m_responseType = value; }
    inline void SetResponseType(GatewayResponseType&& value) { m_responseTypeHasBeenSet = true; m_responseType = std::move(value); }
    inline DeleteGatewayResponseRequest& WithResponseType(const GatewayResponseType& value) { SetResponseType(value); return *this;}
    inline DeleteGatewayResponseRequest& WithResponseType(GatewayResponseType&& value) { SetResponseType(std::move(value)); return *this;}
    ///@}
  private:

    Aws::String m_restApiId;
    bool m_restApiIdHasBeenSet = false;

    GatewayResponseType m_responseType;
    bool m_responseTypeHasBeenSet = false;
  };

} // namespace Model
} // namespace APIGateway
} // namespace Aws
