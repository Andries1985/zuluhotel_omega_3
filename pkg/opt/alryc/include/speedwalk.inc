// Sends the client-side run-speed-modifier packet (0xBF subcommand 0x26).
// Pulled out of the (disabled) objClassMethods package's Character.SpeedWalk()
// so callers don't depend on that package being enabled.
use uo;

function SendSpeedWalk(mobile, toggle := 0)

	var packet := CreatePacket( 0xBF, 6 );
	packet.SetInt16( 1, 6 );
	packet.SetInt16( 3, 0x26 );
	packet.SetInt8( 5, CInt( toggle ) );
	packet.SendPacket( mobile );

	return 1;
endfunction
