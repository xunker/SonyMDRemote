/*
Sony TRS+4-pin remote connector
https://github.com/xunker/SonyMDRemote
*/
trs_d = 3.5;
trs_l = 14;

// space between edge of trs plug and pinblock
trs_pinblock_spacing = 1.85;

pinblock_w = 8; // average

// Width of the plastic carrier around the metal pins
pinblock_outside_t = 1.40; // varies by remote, 1.38 to 1.45

// length of the plastic carrier around the metal pins
pinblock_outside_l = 5; // varies by remote from 4.8 to  5.2

pin_spacing = 2; // on-centre spacing
pin_w = 1.1;
pin_l = 3;
// how deep are the cuts in the pinblock?
pin_top_cut = 0.3;

// include pin labels in render?
// include_pin_labels = true;
include_pin_labels = false;

// rotate 90 degrees?
rotate_90 = true;

ff = 0.01;

module trs() {
  $fn = 24;

  module trs_ring() {
    difference() {
      cylinder(d=trs_d*1.1, h=trs_l/20);
      cylinder(d=trs_d*0.95, h=trs_l/20);
    }
  }

  difference() {
    color("lightgrey") hull() {
      translate([0, 0, trs_l-(trs_d/2)]) sphere(d=trs_d);
      cylinder(d=trs_d, h=trs_l-(trs_d/2));
      // cylinder(d=trs_d, h=trs_l);
    }

    color("green") union() {
       translate([0, 0, trs_l*0.5]) trs_ring();
       translate([0, 0, trs_l*0.7]) trs_ring();
    }
  }
}

module pinblock() {
  difference() {
    color("grey") cube([pinblock_w, pinblock_outside_t, pinblock_outside_l]);

    color("gold") union() {
      pins_x_offset = pinblock_w - (pin_spacing*3);
      translate([pins_x_offset/4, 0, (pinblock_outside_l-pin_l)+ff]) for(pin_no = [0:3]) {
        translate([pin_spacing * pin_no, -ff, 0]) {
          cube([pin_w, pin_top_cut+ff, pin_l+ff]);
          translate([0, ff, pin_l-pin_top_cut]) cube([pin_w, pinblock_outside_t+(ff), pin_top_cut+ff]);
        }
      }
    }
  }

  if (include_pin_labels) {
    pin_labels();
  }
}

module pin_labels() {
  text_size = pin_w;
  text_height = 0.1;

  labels = ["Digital Ground", "Remote (in)", "Data (out)", "Vcc"];
  color("red") for (label_no = [0:3]) {
    label = labels[label_no];
    translate([label_no * pin_spacing, pinblock_outside_t/2, pinblock_outside_l]) translate([text_size, 0, 0]) rotate([90, -90, 0]) linear_extrude(height = text_height) text(label, size = text_size, valign="center");
  }
}

module full_connector() {
  trs();
  translate([(trs_d/2) + trs_pinblock_spacing, 0, 0]) pinblock();
  // just something to hold it all together
  translate([-trs_d, -trs_d, -5]) color("grey") cube([(pinblock_w + trs_d + trs_pinblock_spacing) * 1.25, trs_d * 2, 5]);
}

if (rotate_90) {
  translate([0, 0, (pinblock_w + trs_d + trs_pinblock_spacing) * 0.75])
    rotate([0, 90, 0]) full_connector();
} else {
  full_connector();
}
