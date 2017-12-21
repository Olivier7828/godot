/*************************************************************************/
/*  websocket_server.h                                                   */
/*************************************************************************/
/*                       This file is part of:                           */
/*                      GODOT WEBSOCKET MODULE                           */
/*            https://github.com/LudiDorici/godot-websocket              */
/*************************************************************************/
/* Copyright (c) 2017 Ludi Dorici, di Alessandrelli Fabio                */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include "core/reference.h"
#include "websocket_multiplayer.h"
#include "websocket_peer.h"

class WebSocketServer : public WebSocketMultiplayerPeer {

	GDCLASS(WebSocketServer, WebSocketMultiplayerPeer);
	GDCICLASS(WebSocketServer);

protected:
	static void _bind_methods();

public:
	virtual void poll() = 0;
	virtual Error listen(int p_port, PoolVector<String> p_protocols = PoolVector<String>(), bool gd_mp_api = false) = 0;
	virtual void stop() = 0;
	virtual bool is_listening() const = 0;
	virtual bool has_peer(int p_id) const = 0;
	virtual Ref<WebSocketPeer> get_peer(int p_id) const = 0;
	virtual bool is_server() const;
	ConnectionStatus get_connection_status() const;

	void _on_peer_packet(int32_t p_peer_id);
	void _on_connect(int32_t p_peer_id, String p_protocol);
	void _on_disconnect(int32_t p_peer_id);

	WebSocketServer();
	~WebSocketServer();
};

#endif // WEBSOCKET_H
