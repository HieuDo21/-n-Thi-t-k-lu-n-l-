<?php  
session_start();
?>
<div class="table-responsive" style="max-height: 500px;"> 
  <table class="table">
    <thead class="table-primary">
      <tr>
        <th>ID</th>
        <th>Name</th>
        <th>Serial Number</th>
        <th>Fingerprint ID</th>
        <th>Date</th>
        <th>Time In</th>
        <th>Time Out</th>
      </tr>
    </thead>
    <tbody class="table-secondary">
      <?php

        //Connect to database
        require 'connectDB.php';
        $searchQuery = " ";
        $Start_date = " ";
        $End_date = " ";
        $Start_time = " ";
        $End_time = " ";
        $Finger_sel = " ";

        if (isset($_POST['log_date'])) {
          // Start date filter
          if ($_POST['date_sel_start'] != 0) {
              $Start_date = $_POST['date_sel_start'];
              $_SESSION['searchQuery'] = "checkindate='".$Start_date."'";
          } else {
              $Start_date = date("Y-m-d");
              $_SESSION['searchQuery'] = "checkindate='".date("Y-m-d")."'";
          }
          // End date filter
          if ($_POST['date_sel_end'] != 0) {
              $End_date = $_POST['date_sel_end'];
              $_SESSION['searchQuery'] = "checkindate BETWEEN '".$Start_date."' AND '".$End_date."'";
          }

          // Time-In filter
          if ($_POST['time_sel'] == "Time_in") {
            if (!empty($_POST['time_sel_start']) && !empty($_POST['time_sel_end'])) {
                $Start_time = $_POST['time_sel_start'];
                $End_time = $_POST['time_sel_end'];
                $_SESSION['searchQuery'] .= " AND timein BETWEEN '".$Start_time."' AND '".$End_time."'";
            }
          }
          // Time-out filter
          if ($_POST['time_sel'] == "Time_out") {
            if (!empty($_POST['time_sel_start']) && !empty($_POST['time_sel_end'])) {
                $Start_time = $_POST['time_sel_start'];
                $End_time = $_POST['time_sel_end'];
                $_SESSION['searchQuery'] .= " AND timeout BETWEEN '".$Start_time."' AND '".$End_time."'";
            }
          }

          // Fingerprint filter
          if ($_POST['fing_sel'] != 0) {
              $Finger_sel = $_POST['fing_sel'];
              $_SESSION['searchQuery'] .= " AND fingerprint_id='".$Finger_sel."'";
          }
          
        }
        
        if ($_POST['select_date'] == 1) {
            $Start_date = date("Y-m-d");
            $_SESSION['searchQuery'] = "checkindate='".$Start_date."'";
        }
       
        $sql = "SELECT * FROM users_logs WHERE ".$_SESSION['searchQuery']." ORDER BY id ASC";
        $result = mysqli_stmt_init($conn);
        if (!mysqli_stmt_prepare($result, $sql)) {
            echo '<p class="error">SQL Error</p>';
        } else {
            mysqli_stmt_execute($result);
            $resultl = mysqli_stmt_get_result($result);
            if (mysqli_num_rows($resultl) > 0){
                while ($row = mysqli_fetch_assoc($resultl)){
        ?>
                  <TR>
                  <TD><?php echo $row['id'];?></TD>
                  <TD><?php echo $row['username'];?></TD>
                  <TD><?php echo $row['serialnumber'];?></TD>
                  <TD><?php echo $row['fingerprint_id'];?></TD>
             
                  <TD><?php echo $row['checkindate'];?></TD>
                  <TD><?php echo $row['timein'];?></TD>
                  <TD><?php echo $row['timeout'];?></TD>
                  </TR>
      <?php
                }
            }
        }
        
      ?>
    </tbody>
  </table>
</div>